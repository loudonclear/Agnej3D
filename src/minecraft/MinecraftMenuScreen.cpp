#include "MinecraftMenuScreen.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"
#include "engine/input/Input.h"
#include "minecraft/MinecraftGameScreen.h"
#include "QApplication"
#include "MenuWorld.h"
#include <sstream>


MinecraftMenuScreen::MinecraftMenuScreen()
{
    m_camera = std::make_shared<Camera>();
    m_world = std::make_shared<MenuWorld>();
}

void MinecraftMenuScreen::tick(float seconds) {
    application->captureMouse = false;
    m_world->setApplication(application);
    if (Input::isKeyDown(Qt::Key_Space)) {
        application->setScreen("game");
    }
    m_world->tick(seconds);
}

void MinecraftMenuScreen::draw(Graphics *g) {
    g->setCamera(m_camera);

    m_world->draw(g);
}

void MinecraftMenuScreen::onMousePressed(QMouseEvent *event) {
    m_world->onMousePressed(event);
}

void MinecraftMenuScreen::onMouseReleased(QMouseEvent *event) {
    m_world->onMouseReleased(event);
}

void MinecraftMenuScreen::onMouseMoved(glm::vec2 &delta) {
    m_world->onMouseMoved(delta);
}

void MinecraftMenuScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void MinecraftMenuScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}


