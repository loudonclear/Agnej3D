#include "GraphicsSystem.h"
#include "engine/components/GraphicsComponent.h"
#include "engine/components/UIComponent.h"

#include <algorithm>
#include <iostream>

GraphicsSystem::GraphicsSystem()
{
}

void GraphicsSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<GraphicsComponent> component = go->getComponent<GraphicsComponent>();
    if (component != nullptr) {
        m_components.push_back(component);
    }
    std::shared_ptr<UIComponent> uicomponent = go->getComponent<UIComponent>();
    if (uicomponent != nullptr) {
        m_uiComponents.push_back(uicomponent);
    }
}

void GraphicsSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<GraphicsComponent> component = go->getComponent<GraphicsComponent>();
    if (component != nullptr)
        m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());

    std::shared_ptr<UIComponent> uicomponent = go->getComponent<UIComponent>();
    if (uicomponent != nullptr)
        m_uiComponents.erase(std::remove(m_uiComponents.begin(), m_uiComponents.end(), uicomponent), m_uiComponents.end());
}

void GraphicsSystem::draw(Graphics *g) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->draw(g);
    }
    for (auto iter = m_uiComponents.begin(); iter != m_uiComponents.end(); ++iter) {
        iter->get()->draw(g);
    }
}
