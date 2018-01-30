#include "Transform.h"

Transform::Transform() : m_position(0, 0, 0), m_rotation(0, 0, 0, 1), m_scale(1, 1, 1)
{
}

glm::vec3 Transform::getPosition() {
    return m_position;
}

glm::quat Transform::getRotation() {
    return m_rotation;
}

glm::vec3 Transform::getScale() {
    return m_scale;
}

glm::mat4x4 Transform::getTransformMatrix() {
    if (m_rebuild) {
        glm::mat4x4 trans = glm::translate(-m_position);
        glm::mat4x4 rot = glm::toMat4(m_rotation);
        glm::mat4x4 scale = glm::scale(m_scale);

        m_transformMatrix = glm::transpose(scale * rot * trans);
        m_inverseTransposeMatrix = glm::inverse(glm::transpose(m_transformMatrix));
        m_rebuild = false;
    }

    return m_transformMatrix;
}

glm::mat4x4 Transform::getInverseTransposeMatrix() {
    if (m_rebuild) getTransformMatrix();
    return m_inverseTransposeMatrix;
}

void Transform::setPosition(glm::vec3 position) {
    m_position = position;
    m_rebuild = true;
}

void Transform::setRotation(glm::quat rotation) {
    m_rotation = rotation;
    m_rebuild = true;
}

void Transform::setScale(glm::vec3 scale) {
    m_scale = scale;
    m_rebuild = true;
}

glm::vec3 Transform::transformPoint(const glm::vec3 &point) {
    glm::vec4 result = getTransformMatrix() * glm::vec4(point[0], point[1], point[2], 1.0f);
    return glm::vec3(result[0], result[1], result[2]) / result[3];
}

glm::vec3 Transform::transformVector(const glm::vec3 &vec) {
    glm::vec4 result = getTransformMatrix() * glm::vec4(vec[0], vec[1], vec[2], 0.0f);
    return glm::vec3(result[0], result[1], result[2]);
}

Ray Transform::transformRay(const Ray &ray) {
    return Ray(transformPoint(ray.start), transformVector(ray.direction));
}

glm::vec3 Transform::transformNormal(const glm::vec3 &normal) {
    glm::vec4 result = getInverseTransposeMatrix() * glm::vec4(normal[0], normal[1], normal[2], 0.0f);
    return glm::vec3(result[0], result[1], result[2]);
}
