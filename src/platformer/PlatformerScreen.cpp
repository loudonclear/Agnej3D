#include "PlatformerScreen.h"

#include "engine/graphics/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/input/Input.h"
#include "engine/world/World.h"
#include "PlatformerWorld.h"
#include "view.h"
#include <QApplication>

std::shared_ptr<Camera> PlatformerScreen::camera;

PlatformerScreen::PlatformerScreen(std::string level)
{
    camera = std::make_shared<Camera>();
    m_world = std::make_shared<PlatformerWorld>(level);
}

void PlatformerScreen::tick(float seconds) {
    application->captureMouse = true;
    m_world->tick(seconds);
}

void PlatformerScreen::draw(Graphics *g) {
    g->setCamera(camera);
    m_world->draw(g);
}

void PlatformerScreen::resize(glm::vec2 &size) {
    camera->setScreenSize(size);
}

void PlatformerScreen::onMousePressed(QMouseEvent *event) {
    m_world->onMousePressed(event);
}

void PlatformerScreen::onMouseReleased(QMouseEvent *event) {
    m_world->onMouseReleased(event);
}

void PlatformerScreen::onMouseMoved(glm::vec2 &delta) {
    m_world->onMouseMoved(delta);
}

void PlatformerScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void PlatformerScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        application->setScreen("menu");
    }
}
