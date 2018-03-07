#include "HealthBar.h"
#include "engine/world/GameObject.h"
#include "engine/graphics/Camera.h"

HealthBar::HealthBar(GameObject *parent, const std::string &materialName, glm::vec3 offset, glm::vec3 scale) : Component(parent), UIComponent(parent, materialName), HUDComponent(parent, materialName), m_offset(offset), m_scale(scale)
{
    m_health = 1;
}

void HealthBar::draw(Graphics *g) {
    HUDComponent::drawBegin(g);

    g->clearTransform();
    g->setMaterial(m_materialName);
    g->translate(m_offset);
    g->scale(glm::vec3(m_scale.x * m_health, m_scale.y, 1));
    g->drawShape("uiquad");

    HUDComponent::drawEnd(g);
}

void HealthBar::setHealth(float health) {
    m_health = health;
}

float HealthBar::getHealth() {
    return m_health;
}
