#include "PlatformerMenuScreen.h"

#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"
#include "engine/input/Input.h"
#include "minecraft/MinecraftGameScreen.h"
#include "QApplication"
#include "MenuWorld.h"
#include <sstream>


PlatformerMenuScreen::PlatformerMenuScreen()
{
    m_camera = std::make_shared<Camera>();
    m_world = std::make_shared<MenuWorld>();
}

void PlatformerMenuScreen::tick(float seconds) {
    application->captureMouse = false;
    m_world->tick(seconds);
}

void PlatformerMenuScreen::draw(Graphics *g) {
    g->setCamera(m_camera);

    m_world->draw(g);
}

void PlatformerMenuScreen::onMousePressed(QMouseEvent *event) {
    m_world->onMousePressed(event);
}

void PlatformerMenuScreen::onMouseReleased(QMouseEvent *event) {
    m_world->onMouseReleased(event);
}

void PlatformerMenuScreen::onMouseMoved(glm::vec2 &delta) {
    m_world->onMouseMoved(delta);
}

void PlatformerMenuScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void PlatformerMenuScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}


