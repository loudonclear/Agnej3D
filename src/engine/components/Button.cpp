#include "Button.h"
#include "engine/input/Input.h"

#include <iostream>
#include "engine/graphics/Camera.h"

Button::Button(GameObject *parent, const std::string &materialName, glm::vec2 offset, glm::vec2 length) : Component(parent), TickComponent(parent), InputComponent(parent), UIComponent(parent, materialName), HUDComponent(parent, materialName), m_offset(offset), m_length(length)
{
}

void Button::tick(float seconds) {
    glm::vec2 mp = Input::getMousePosition();

    if (mp.x >= m_offset.x && mp.x <= m_offset.x + m_length.x && mp.y >= m_offset.y && mp.y <= m_offset.y + m_length.y) {
        canClick = true;
    } else {
        canClick = false;
    }
}

void Button::onMouseReleased(QMouseEvent *event) {
    if (canClick) {
        onClick();
    }
}

void Button::draw(Graphics *g) {
    HUDComponent::drawBegin(g);

    g->clearTransform();
    g->setMaterial(m_materialName);
    g->translate(glm::vec3(m_offset.x, 1 - m_offset.y - m_length.y, 0));
    g->scale(glm::vec3(m_length.x, m_length.y, 1));

    g->drawShape("uiquad");

    HUDComponent::drawEnd(g);
}
