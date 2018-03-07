#include "MinecraftGameScreen.h"

#include "engine/graphics/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/input/Input.h"
#include "engine/world/World.h"
#include "minecraft/MinecraftWorld.h"
#include "view.h"

std::shared_ptr<Camera> MinecraftGameScreen::camera;

MinecraftGameScreen::MinecraftGameScreen()
{
    camera = std::make_shared<Camera>();
    m_world = std::make_shared<MinecraftWorld>();
}

void MinecraftGameScreen::tick(float seconds) {
    application->captureMouse = true;
    m_world->tick(seconds);
}

void MinecraftGameScreen::draw(Graphics *g) {
    g->setCamera(camera);
    m_world->draw(g);
}

void MinecraftGameScreen::resize(glm::vec2 &size) {
    camera->setScreenSize(size);
}

void MinecraftGameScreen::onMousePressed(QMouseEvent *event) {
    m_world->onMousePressed(event);
}

void MinecraftGameScreen::onMouseReleased(QMouseEvent *event) {
    m_world->onMouseReleased(event);
}

void MinecraftGameScreen::onMouseMoved(glm::vec2 &delta) {
    m_world->onMouseMoved(delta);
}

void MinecraftGameScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void MinecraftGameScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_world = std::make_shared<MinecraftWorld>();
        application->setScreen("menu");
    }
}
