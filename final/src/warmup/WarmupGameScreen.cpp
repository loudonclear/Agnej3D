#include "WarmupGameScreen.h"
#include "engine/graphics/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/input/Input.h"
#include "engine/world/World.h"
#include "warmup/WarmupGameWorld.h"

std::shared_ptr<Camera> WarmupGameScreen::camera;

WarmupGameScreen::WarmupGameScreen()
{
    camera = std::make_shared<Camera>();
    m_world = std::make_shared<WarmupGameWorld>();
}

void WarmupGameScreen::tick(float seconds) {
    m_world->tick(seconds);
}

void WarmupGameScreen::draw(Graphics *g) {
    g->setCamera(camera);
    m_world->draw(g);
}

void WarmupGameScreen::resize(glm::vec2 &size) {
    camera->setScreenSize(size);
}


void WarmupGameScreen::onMouseMoved(glm::vec2 &delta) {
    m_world->onMouseMoved(delta);
}

void WarmupGameScreen::onMouseWheelMoved(QWheelEvent *event) {
    m_world->onMouseWheelMoved(event);
}

void WarmupGameScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        m_world = std::make_shared<WarmupGameWorld>();
        application->setScreen("menu");
    } else if (event->key() == Qt::Key_R) {
        m_world = std::make_shared<WarmupGameWorld>();
    }
}
