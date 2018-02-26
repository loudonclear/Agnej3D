#include "InputSystem.h"


InputSystem::InputSystem()
{

}

void InputSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<InputComponent> component = go->getComponent<InputComponent>();
    if (component != nullptr) {
        m_components.push_back(component);
    }
}

void InputSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<InputComponent> component = go->getComponent<InputComponent>();
    if (component != nullptr)
        m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void InputSystem::onKeyPressed(QKeyEvent *event) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->onKeyPressed(event);
    }
}

void InputSystem::onKeyReleased(QKeyEvent *event) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->onKeyReleased(event);
    }
}

void InputSystem::onMousePressed(QMouseEvent *event) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->onMousePressed(event);
    }
}

void InputSystem::onMouseReleased(QMouseEvent *event) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->onMouseReleased(event);
    }
}

void InputSystem::onMouseMoved(glm::vec2 &delta) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->onMouseMoved(delta);
    }
}

void InputSystem::onMouseWheelMoved(QWheelEvent *event) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->onMouseWheelMoved(event);
    }
}
