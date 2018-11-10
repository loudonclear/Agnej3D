#include "GraphicsComponent.h"
#include "engine/world/GameObject.h"
#include "engine/graphics/Graphics.h"

GraphicsComponent::GraphicsComponent(GameObject *parent, const std::string &materialName) : Component(parent), m_materialName(materialName), m_transform(nullptr)
{
}

void GraphicsComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
}
