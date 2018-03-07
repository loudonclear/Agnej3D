#include "HUDComponent.h"

#include "engine/world/GameObject.h"
#include "engine/graphics/Camera.h"

HUDComponent::HUDComponent(GameObject *parent, const std::string &materialName) : Component(parent), UIComponent(parent, materialName)
{
}

void HUDComponent::drawBegin(Graphics *g) {
    g->getActiveCamera()->setUI(true);
    g->setCamera(g->getActiveCamera());
    g->clearTransform();
}

void HUDComponent::drawEnd(Graphics *g) {
    g->getActiveCamera()->setUI(false);
    g->setCamera(g->getActiveCamera());
}
