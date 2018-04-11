#include "WarmupDebugScreen.h"
#include "engine/graphics/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/input/Input.h"
#include "engine/world/World.h"
#include "warmup/WarmupDebugWorld.h"

WarmupDebugScreen::WarmupDebugScreen()
{
    m_world = std::make_shared<WarmupDebugWorld>();

    m_camera = std::make_shared<Camera>();
    m_camera->setEye(glm::vec3(0, 3, -7));
    m_camera->setLook(glm::vec3(0, 0, 1));
}

void WarmupDebugScreen::tick(float seconds) {
    glm::vec3 look = m_camera->getLook();

    glm::vec3 dir = glm::normalize(glm::vec3(look.x, look.y, look.z));
    glm::vec3 perp = glm::vec3(dir.z, 0, -dir.x);

    if (Input::isKeyDown(Qt::Key_W)) m_camera->translate(dir*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_S)) m_camera->translate(-dir*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_A)) m_camera->translate(perp*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_D)) m_camera->translate(-perp*seconds*5.f);

    m_world->tick(seconds);
}

void WarmupDebugScreen::draw(Graphics *g) {

    g->setCamera(m_camera);

    g->clearTransform();
    g->scale(20);
    g->setMaterial("grass");
    g->drawShape("quad");

    m_world->draw(g);
}

void WarmupDebugScreen::resize(glm::vec2 &size) {
    m_camera->setScreenSize(size);
}


void WarmupDebugScreen::onMouseMoved(glm::vec2 &delta) {
    m_camera->rotate(-delta.x / 100.f, -delta.y / 100.f);
    m_world->onMouseMoved(delta);
}

void WarmupDebugScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void WarmupDebugScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_world = std::make_shared<WarmupDebugWorld>();
        application->setScreen("menu");
    }
    m_world->onKeyPressed(event);
}
