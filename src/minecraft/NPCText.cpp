#include "NPCText.h"
#include "engine/world/GameObject.h"
#include "engine/graphics/Camera.h"

NPCText::NPCText(GameObject *parent, const std::string &materialName, glm::vec3 offset, std::string text) : Component(parent), UIComponent(parent, materialName), OrthographicUIComponent(parent, materialName), m_offset(offset), m_text(text)
{
}

void NPCText::draw(Graphics *g) {
    OrthographicUIComponent::drawBegin(g);

    float lineSize = .1;
    glm::vec3 pos = g->getActiveCamera()->convertToScreenSpace(m_transform->getPosition() + m_offset);
    if (pos.z > 0) {
        g->translate(glm::vec3(pos.x - 0.05f, pos.y, 0));
        g->setMaterial(m_materialName);
        g->drawText(m_text, lineSize);
    }

    OrthographicUIComponent::drawEnd(g);
}

void NPCText::setText(std::string text) {
    m_text = text;
}
