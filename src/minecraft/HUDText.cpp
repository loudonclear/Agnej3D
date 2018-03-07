#include "HUDText.h"

#include "engine/world/GameObject.h"
#include "engine/graphics/Camera.h"

HUDText::HUDText(GameObject *parent, const std::string &materialName, glm::vec3 offset, float scale, std::string text) : Component(parent), UIComponent(parent, materialName), HUDComponent(parent, materialName), m_offset(offset), m_scale(scale), m_text(text)
{
}

void HUDText::draw(Graphics *g) {
    HUDComponent::drawBegin(g);

    g->clearTransform();
    g->setMaterial(m_materialName);
    g->translate(m_offset);
    g->setMaterial(m_materialName);
    g->drawText(m_text, m_scale);

    HUDComponent::drawEnd(g);
}
