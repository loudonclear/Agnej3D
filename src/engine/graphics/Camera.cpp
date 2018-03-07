#include "Camera.h"

Camera::Camera(glm::vec2 screenSize, glm::vec3 eye, float yaw, float pitch, float fov) :
    m_yaw(yaw),
    m_pitch(pitch),
    m_fov(fov),
    m_screenSize(screenSize),
    m_eye(eye),
    m_up(glm::vec3(0, 1, 0)),
    m_forward(glm::vec3(0, 0, 1)),
    m_ui(false),
    m_inverted(false),
    m_viewDirty(true),
    m_projDirty(true),
    m_cullDirty(true)
{
}

Camera::~Camera()
{
}

glm::vec2 Camera::getScreenSize()
{
    return m_screenSize;
}

void Camera::setScreenSize(glm::vec2 size)
{
    m_screenSize = size;
    m_projDirty = true;
    m_cullDirty = true;
}

float Camera::getYaw()
{
    return m_yaw;
}

void Camera::setYaw(float yaw)
{
    m_yaw = yaw;
    m_viewDirty = true;
    m_cullDirty = true;
}

float Camera::getPitch()
{
    return m_pitch;
}

void Camera::setPitch(float pitch)
{
    m_pitch = pitch;
}

float Camera::getFov()
{
    return m_fov;
}

void Camera::setFov(float fov)
{
    m_fov = fov;
    m_projDirty = true;
    m_cullDirty = true;
}

glm::vec3 Camera::getEye()
{
    return m_eye;
}

void Camera::setEye(glm::vec3 eye)
{
    m_eye = eye;
    m_viewDirty = true;
    m_cullDirty = true;
}

glm::vec3 Camera::getLook()
{
    glm::vec3 left = glm::normalize(glm::cross(m_up, m_forward));
    glm::vec3 horizontal = glm::cos(m_yaw) * m_forward + glm::sin(m_yaw) * left;
    return glm::cos(m_pitch) * horizontal + glm::sin(m_pitch) * m_up;
}

void Camera::setLook(glm::vec3 look) {
    look = glm::normalize(look);
    glm::vec3 left = glm::normalize(glm::cross(m_up, m_forward));

    float hx = glm::dot(look, m_forward);
    float hy = glm::dot(look, left);
    m_yaw = atan2f(hy, hx);

    float vy = glm::dot(look, m_up);
    m_pitch = (glm::pi<float>() / 2.f) - acosf(vy);
    m_pitch = glm::clamp(static_cast<double>(m_pitch), -M_PI / 2.0 + 0.01, M_PI / 2.0 - 0.01);

    m_viewDirty = true;
    m_cullDirty = true;
}

glm::vec3 Camera::getUp() {
    return m_up;
}

void Camera::setUp(glm::vec3 up) {
    m_up = glm::normalize(up);
    m_viewDirty = true;
    m_cullDirty = true;
}

glm::vec3 Camera::getForward() {
    return m_forward;
}

void Camera::setForward(glm::vec3 forward) {
    m_forward = glm::normalize(forward);
    m_viewDirty = true;
    m_cullDirty = true;
}

bool Camera::isUI() {
    return m_ui;
}

void Camera::setUI(bool ui) {
    m_ui = ui;
}

bool Camera::isInverted() {
    return m_inverted;
}

void Camera::setInverted(bool inverted) {
    m_inverted = inverted;
}

void Camera::translate(glm::vec3 vec)
{
    m_eye += vec;
    m_viewDirty = true;
    m_cullDirty = true;
}

void Camera::rotate(float yaw, float pitch)
{
    m_yaw += yaw;
    m_pitch += pitch;
    m_pitch = glm::clamp(static_cast<double>(m_pitch), -M_PI / 2.0 + 0.01, M_PI / 2.0 - 0.01);

    m_viewDirty = true;
    m_cullDirty = true;
}

glm::mat4 Camera::getView() {
    if (m_viewDirty) {
        m_view = glm::lookAt(m_eye, m_eye + getLook(), m_up);
        m_viewDirty = false;
    }
    return m_view;
}

glm::mat4 Camera::getProjection() {
    if (m_projDirty) {
        m_proj = glm::perspective(m_fov, m_screenSize.x / m_screenSize.y,
                                      nearPlane, farPlane);
        m_projDirty = false;
    }
    return m_proj;
}

bool Camera::frustumCull(std::shared_ptr<ShapeCollider> sc) {
    if (m_cullDirty) {
        glm::mat4x4 projView = getProjection() * getView();

        glm::vec4 r0 = glm::vec4(projView[0][0], projView[1][0], projView[2][0], projView[3][0]);
        glm::vec4 r1 = glm::vec4(projView[0][1], projView[1][1], projView[2][1], projView[3][1]);
        glm::vec4 r2 = glm::vec4(projView[0][2], projView[1][2], projView[2][2], projView[3][2]);
        glm::vec4 r3 = glm::vec4(projView[0][3], projView[1][3], projView[2][3], projView[3][3]);

        m_clipPlanes[0] = r3 - r0;
        m_clipPlanes[1] = r3 - r1;
        m_clipPlanes[2] = r3 - r2;
        m_clipPlanes[3] = r3 + r0;
        m_clipPlanes[4] = r3 + r1;
        m_clipPlanes[5] = r3 + r2;

        for (int i = 0; i < 6; i++) {
            m_clipPlanes[i] /= glm::length(glm::vec3(m_clipPlanes[i]));
        }

        m_cullDirty = false;
    }

    for (glm::vec4 plane : m_clipPlanes) {
        glm::vec3 normal = glm::vec3(plane);
        glm::vec3 p = sc->getSupport(normal);

        if (glm::dot(normal, p) + plane.w < 0) {
            return true;
        }
    }

    return false;
}

glm::vec3 Camera::convertToScreenSpace(glm::vec3 pos) {
    glm::vec4 fourVec = glm::vec4(pos.x, pos.y, pos.z, 1);
    fourVec = getProjection() * getView() * fourVec;
    glm::vec3 clipSpace = glm::vec3(fourVec.x / fourVec.w, fourVec.y / fourVec.w, fourVec.z);

    return glm::vec3((clipSpace.x + 1) * 0.5f * m_screenSize.x, (1 - (1 - clipSpace.y) * 0.5f * m_screenSize.y), clipSpace.z);
}

glm::mat4 Camera::getUIView() {
    return glm::mat4();
}

glm::mat4 Camera::getUIProjection() {
    glm::mat4 proj;

    // Scale
    if(m_inverted) {
        proj = glm::scale(proj, glm::vec3(2.f / m_screenSize.x, -2.f / m_screenSize.y, 1));
    }
    else {
        proj = glm::scale(proj, glm::vec3(2.f / m_screenSize.x, 2.f / m_screenSize.y, 1));
    }

    // Translate
    proj = glm::translate(proj, glm::vec3(-m_screenSize.x / 2.f, -m_screenSize.y / 2.f, 0));

    return proj;
}
