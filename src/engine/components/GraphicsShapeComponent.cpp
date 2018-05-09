#include "GraphicsShapeComponent.h"
#include "engine/components/Transform.h"
#include "engine/world/GameObject.h"
#include "engine/graphics/Camera.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

GraphicsShapeComponent::GraphicsShapeComponent(GameObject *parent, const std::string &shapeName, const std::string &materialName, bool frustumCull) :
    Component(parent), GraphicsComponent(parent, materialName), m_frustumCull(frustumCull)
{
    Graphics *g = Graphics::getGlobalInstance();
    m_shape = g->getShape(shapeName);
}

GraphicsShapeComponent::GraphicsShapeComponent(GameObject *parent, std::shared_ptr<Shape> shape, const std::string &materialName, bool frustumCull) :
    Component(parent), GraphicsComponent(parent, materialName), m_shape(shape), m_frustumCull(frustumCull)
{
}

void GraphicsShapeComponent::init() {
    GraphicsComponent::init();
}

void GraphicsShapeComponent::draw(Graphics *g) {

    if (m_shape != nullptr) {
        g->clearTransform();
        g->setMaterial(m_materialName);
        g->setTransform(m_transform->getTransformMatrix());
        g->drawShape(m_shape);
    }
}
