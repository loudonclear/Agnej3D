#include "Button.h"
#include "engine/input/Input.h"

#include <iostream>

Button::Button(GameObject *parent, const std::string &materialName, glm::ivec2 offset, glm::ivec2 length) : Component(parent), TickComponent(parent), InputComponent(parent), UIComponent(parent, materialName), HUDComponent(parent, materialName), m_offset(offset), m_length(length)
{
}

void Button::tick(float seconds) {
    glm::ivec2 mp = Input::getMousePosition();
    std::cout << "here" << std::endl;
    if (mp.x >= m_offset.x && mp.x <= m_offset.x + m_length.x && mp.y >= m_offset.y && mp.y <= m_offset.y + m_length.y) {
        onClick();
    }
}

void Button::onClick() {
    std::cout << "asdsad" << std::endl;
}

void Button::draw(Graphics *g) {
    HUDComponent::drawBegin(g);

    g->clearTransform();
    g->setMaterial(m_materialName);
    g->translate(glm::vec3(m_offset.x, m_offset.y, 0));
    g->scale(glm::vec3(0.1f, 0.1f, 1));
    g->drawShape("uiquad");

    HUDComponent::drawEnd(g);
}
