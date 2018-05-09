#include "Application.h"
#include "graphics/Graphics.h"
#include "Screen.h"


Application::Application() : m_activeScreen(nullptr)
{

}

void Application::addScreen(const std::string &name, const std::shared_ptr<Screen> &screen) {
    m_screens[name] = screen;
    screen->setApplication(this);
}

void Application::setScreen(const std::string &name) {
    m_activeScreen = m_screens[name];
}

void Application::removeScreen(const std::string &name) {
    m_screens.erase(m_screens.find(name));
}

std::shared_ptr<Screen> Application::getActiveScreen() {
    return m_activeScreen;
}


void Application::tick(float seconds) {
    m_activeScreen->tick(seconds);
}

void Application::draw(Graphics *g) {
    m_activeScreen->draw(g);
}

void Application::resize(glm::vec2 size) {
    m_activeScreen->resize(size);
}

void Application::onKeyPressed(QKeyEvent *event) {
    m_activeScreen->onKeyPressed(event);
}

void Application::onKeyReleased(QKeyEvent *event) {
    m_activeScreen->onKeyReleased(event);
}

void Application::onMousePressed(QMouseEvent *event) {
    m_activeScreen->onMousePressed(event);
}

void Application::onMouseReleased(QMouseEvent *event) {
    m_activeScreen->onMouseReleased(event);
}

void Application::onMouseMoved(glm::vec2 delta) {
    m_activeScreen->onMouseMoved(delta);
}

void Application::onMouseWheelMoved(QWheelEvent *event) {
    m_activeScreen->onMouseWheelMoved(event);
}

