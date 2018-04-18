#include "MenuButton.h"
#include "engine/input/Input.h"

#include <iostream>
#include "engine/graphics/Camera.h"
#include "engine/Application.h"
#include "platformer/PlatformerScreen.h"
#include "view.h"

MenuButton::MenuButton(GameObject *parent, std::string text, const std::string &materialName, glm::vec2 offset, glm::vec2 length) : Component(parent), TickComponent(parent), InputComponent(parent), UIComponent(parent, materialName), HUDComponent(parent, materialName), m_text(text), m_offset(offset), m_length(length)
{
}

void MenuButton::tick(float seconds) {
    glm::vec2 mp = Input::getMousePosition();

    if (mp.x >= m_offset.x && mp.x <= m_offset.x + m_length.x && mp.y >= m_offset.y && mp.y <= m_offset.y + m_length.y) {
        canClick = true;
    } else {
        canClick = false;
    }
}

void MenuButton::onMouseReleased(QMouseEvent *event) {
    if (canClick) {
        onClick();
    }
}

void MenuButton::onClick() {
    View::app->addScreen(m_text, std::make_shared<PlatformerScreen>());
    View::app->setScreen(m_text);
}

void MenuButton::draw(Graphics *g) {
    HUDComponent::drawBegin(g);

    g->clearTransform();

    g->translate(glm::vec3(m_offset.x, 1 - m_offset.y - m_length.y, 0));
    g->scale(glm::vec3(m_length.x, m_length.y, 1));

    g->setMaterial(m_materialName);
    g->drawShape("uiquad");

    g->setDefaultMaterial();
    g->clearTransform();
    g->translate(glm::vec3(m_offset.x + m_length.x / 4, 1 - m_offset.y - m_length.y / 2, 0));
    g->drawText(m_text, 0.1f);

    HUDComponent::drawEnd(g);
}

