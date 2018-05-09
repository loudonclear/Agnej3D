#include "FinalMenuScreen.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"
#include "engine/input/Input.h"
#include "QApplication"


FinalMenuScreen::FinalMenuScreen()
{
    m_camera = std::make_shared<Camera>();
}

void FinalMenuScreen::tick(float seconds) {
    if (Input::isKeyDown(Qt::Key_Space)) {
        application->setScreen("debug");
    }
}

void FinalMenuScreen::draw(Graphics *g) {

    m_camera->setUI(true);
    g->setCamera(m_camera);
    g->setDefaultMaterial();


    g->clearTransform();
    g->translate(glm::vec3(0.02f, 0.4f, 0));
    g->drawText("default", "Press Space for debug mode", 0.1f);
}

void FinalMenuScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}


