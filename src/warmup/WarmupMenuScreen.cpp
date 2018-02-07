#include "WarmupMenuScreen.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"
#include "engine/input/Input.h"
#include "QApplication"


WarmupMenuScreen::WarmupMenuScreen()
{
    m_camera = std::make_shared<Camera>();
}

void WarmupMenuScreen::tick(float seconds) {
    if (Input::isKeyDown(Qt::Key_Space)) {
        application->setScreen("game");
    } else if (Input::isKeyDown(Qt::Key_Semicolon)) {
        application->setScreen("debug");
    }
}

void WarmupMenuScreen::draw(Graphics *g) {

    m_camera->setUI(true);
    g->setCamera(m_camera);
    g->setDefaultMaterial();

    g->clearTransform();
    g->translate(glm::vec3(0.15f, 0.5f, 0));
    g->drawText("default", "Press Space to start", 0.1f);

    g->clearTransform();
    g->translate(glm::vec3(0.02f, 0.4f, 0));
    g->drawText("default", "Press ; to enter debug mode", 0.1f);
}

void WarmupMenuScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}


