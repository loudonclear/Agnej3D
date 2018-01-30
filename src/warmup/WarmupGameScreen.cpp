#include "WarmupGameScreen.h"
#include "engine/graphics/Camera.h"
#include "engine/graphics/Graphics.h"
#include "engine/input/Input.h"

WarmupGameScreen::WarmupGameScreen()
{
    Graphics *graphics = Graphics::getGlobalInstance();

    m_camera = std::make_shared<Camera>();
    m_camera->setEye(glm::vec3(0, 1, 0));
    graphics->setCamera(m_camera);

    Material grass;
    grass.textureName = "grass";
    grass.textureRepeat = glm::vec2(10, 10);
    graphics->addMaterial("grass", grass);
}

void WarmupGameScreen::tick(float seconds) {
    glm::vec3 look = m_camera->getLook();

    glm::vec3 dir = glm::normalize(glm::vec3(look.x, 0, look.z));
    glm::vec3 perp = glm::vec3(dir.z, 0, -dir.x);

    if (Input::isKeyDown(Qt::Key_W)) m_camera->translate(dir*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_S)) m_camera->translate(-dir*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_A)) m_camera->translate(perp*seconds*5.f);
    if (Input::isKeyDown(Qt::Key_D)) m_camera->translate(-perp*seconds*5.f);

    glm::vec3 eye = m_camera->getEye();
    m_velocity += glm::vec3(0, -2, 0);
    glm::vec3 pos = eye + m_velocity * seconds;

    if (pos.y <= 1) {
        m_velocity = glm::vec3();
        m_camera->setEye(glm::vec3(eye.x, 1, eye.z));

        if (Input::isKeyDown(Qt::Key_Space)) {
            m_velocity += glm::vec3(0, 30, 0);
        }
    } else {
        m_camera->setEye(pos);
    }

}

void WarmupGameScreen::draw(Graphics *g) {

    g->setCamera(m_camera);

    g->clearTransform();
    g->scale(20);
    g->setMaterial("grass");
    g->drawShape("quad");
}

void WarmupGameScreen::resize(glm::vec2 &size) {
    m_camera->setScreenSize(size);
}


void WarmupGameScreen::onMousePressed(QMouseEvent *event) {

}

void WarmupGameScreen::onMouseMoved(glm::vec2 &delta) {
    m_camera->rotate(-delta.x / 100.f, -delta.y / 100.f);
}

void WarmupGameScreen::onMouseReleased(QMouseEvent *event) {

}

void WarmupGameScreen::onMouseWheelMoved(QWheelEvent *event) {

    //m_camera->translate(event->delta() / 120.f * m_camera->getLook());
}

void WarmupGameScreen::onKeyPressed(QKeyEvent *event) {

}

void WarmupGameScreen::onKeyReleased(QKeyEvent *event) {

}
