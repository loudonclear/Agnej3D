#include "FinalMenuScreen.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"
#include "engine/input/Input.h"
#include "QApplication"
#include <string>
#include <sstream>

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

    glm::vec2 screenSize = m_camera->getScreenSize();
    m_camera->setScreenSize(glm::vec2(1));
    m_camera->setUI(true);
    g->setCamera(m_camera);
    g->setDefaultMaterial();

    g->clearTransform();
    g->translate(glm::vec3(0.29f, 0.5f, 0));
    g->drawText("dragon", "AGNEJ", 0.2f);

    g->clearTransform();
    g->translate(glm::vec3(0.16f, 0.35f, 0));
    g->drawText("dragon", "Press Space to begin", 0.1f);

    m_camera->setUI(false);
    m_camera->setScreenSize(screenSize);
    g->setCamera(m_camera);
}

void FinalMenuScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) QApplication::quit();
}

void FinalMenuScreen::resize(glm::vec2 &size) {
    m_camera->setScreenSize(size);
    Graphics::getGlobalInstance()->setCamera(m_camera);
}

