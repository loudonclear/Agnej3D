#include "GraphicsSystem.h"
#include "engine/components/GraphicsComponent.h"

#include <algorithm>

GraphicsSystem::GraphicsSystem()
{
}

void GraphicsSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<GraphicsComponent> component = go->getComponent<GraphicsComponent>();
    if (component != nullptr) m_components.push_back(component);
}

void GraphicsSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<GraphicsComponent> component = go->getComponent<GraphicsComponent>();
    if (component != nullptr)
        m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void GraphicsSystem::draw(Graphics *g) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->draw(g);
    }
}
