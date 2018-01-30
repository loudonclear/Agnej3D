#include "WarmupMenuScreen.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Camera.h"



WarmupMenuScreen::WarmupMenuScreen()
{
    Graphics *graphics = Graphics::getGlobalInstance();

    m_camera = std::make_shared<Camera>();
    m_camera->setEye(glm::vec3(0, 1, 25));
    m_camera->setLook(glm::vec3(0, 0, -1));
}

void WarmupMenuScreen::draw(Graphics *g) {

    g->setCamera(m_camera);

    g->clearTransform();
    g->translate(glm::vec3(-13.5f, 0, 0));
    g->setDefaultMaterial();
    g->drawText("Press Enter to begin", 4);
}

void WarmupMenuScreen::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_Enter) {
        application->setScreen("game");
    }
}


