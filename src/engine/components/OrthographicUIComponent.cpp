#include "OrthographicUIComponent.h"

#include "engine/world/GameObject.h"
#include "engine/graphics/Camera.h"

OrthographicUIComponent::OrthographicUIComponent(GameObject *parent, const std::string &materialName) : Component(parent), UIComponent(parent, materialName)
{
}

void OrthographicUIComponent::drawBegin(Graphics *g) {
    screenSize = g->getActiveCamera()->getScreenSize();
    g->getActiveCamera()->setScreenSize(glm::vec2(1));
    g->getActiveCamera()->setUI(true);
    g->setCamera(g->getActiveCamera());
    g->clearTransform();
}

void OrthographicUIComponent::drawEnd(Graphics *g) {
    g->getActiveCamera()->setUI(false);
    g->getActiveCamera()->setScreenSize(screenSize);
    g->setCamera(g->getActiveCamera());
}
