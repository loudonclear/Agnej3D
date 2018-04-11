#include "GraphicsComponent.h"
#include "engine/world/GameObject.h"
#include "engine/graphics/Graphics.h"

GraphicsComponent::GraphicsComponent(std::shared_ptr<GameObject> &parent, const std::string &materialName) : Component(parent), m_materialName(materialName)
{
}

void GraphicsComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
}
