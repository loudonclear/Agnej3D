#include "FinalDebugScreen.h"
#include "engine/graphics/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/input/Input.h"
#include "engine/world/World.h"
#include "game/FinalDebugWorld.h"

FinalDebugScreen::FinalDebugScreen(const std::vector<std::string> &shapes) : shapes(shapes)
{
    m_world = std::make_shared<FinalDebugWorld>(shapes);

    m_camera = std::make_shared<Camera>();
    m_camera->setEye(glm::vec3(0, 3, -7));
    m_camera->setLook(glm::vec3(0, 0, 1));
}

void FinalDebugScreen::tick(float seconds) {

    glm::vec3 dir = glm::normalize(m_camera->getLook());
    glm::vec3 perp = glm::normalize(glm::cross(m_camera->getUp(), m_camera->getLook()));

    if (Input::isKeyDown(Qt::Key_W)) m_camera->translate(dir*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_S)) m_camera->translate(-dir*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_A)) m_camera->translate(perp*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_D)) m_camera->translate(-perp*seconds*5.f);

    m_world->tick(seconds);
}

void FinalDebugScreen::draw(Graphics *g) {

    g->setCamera(m_camera);
    g->clearTransform();

    m_world->draw(g);
}

void FinalDebugScreen::resize(glm::vec2 &size) {
    m_camera->setScreenSize(size);
}

void FinalDebugScreen::onMousePressed(QMouseEvent *event) {
    m_world->onMousePressed(event);
}

void FinalDebugScreen::onMouseReleased(QMouseEvent *event) {
    m_world->onMouseReleased(event);
}

void FinalDebugScreen::onMouseMoved(glm::vec2 &delta) {
    m_camera->rotate(-delta.x / 100.f, -delta.y / 100.f);
    m_world->onMouseMoved(delta);
}

void FinalDebugScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void FinalDebugScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_world = std::make_shared<FinalDebugWorld>(shapes);
        application->setScreen("menu");
    }
    m_world->onKeyPressed(event);
}

void FinalDebugScreen::onKeyReleased(QKeyEvent *event) {
    m_world->onKeyReleased(event);
}

