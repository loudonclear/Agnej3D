#include "UIComponent.h"
#include "engine/world/GameObject.h"

UIComponent::UIComponent(GameObject *parent, const std::string &materialName) : Component(parent), m_materialName(materialName)
{
}

void UIComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
}
