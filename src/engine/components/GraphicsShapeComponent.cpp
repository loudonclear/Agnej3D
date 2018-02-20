#include "GraphicsShapeComponent.h"
#include "engine/components/Transform.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/Collision.h"
#include "engine/graphics/Camera.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

GraphicsShapeComponent::GraphicsShapeComponent(GameObject *parent, const std::string &shapeName, const std::string &materialName, bool frustumCull) :
    GraphicsComponent(parent, materialName), m_frustumCull(frustumCull)
{
    Graphics *g = Graphics::getGlobalInstance();
    m_shape = g->getShape(shapeName);
}

GraphicsShapeComponent::GraphicsShapeComponent(GameObject *parent, std::shared_ptr<Shape> shape, const std::string &materialName, bool frustumCull) :
    GraphicsComponent(parent, materialName), m_shape(shape), m_frustumCull(frustumCull)
{
}

void GraphicsShapeComponent::init() {
    m_shapeCollider = m_gameObject->getComponent<ShapeCollider>();
    if (!m_shapeCollider) m_frustumCull = false;
    GraphicsComponent::init();
}

void GraphicsShapeComponent::draw(Graphics *g) {

    if (m_frustumCull) {
        if (!g->getActiveCamera()->frustumCull(m_shapeCollider)) {
            g->clearTransform();
            g->setMaterial(m_materialName);
            g->setTransform(m_transform->getTransformMatrix());
            g->drawShape(m_shape);
        }
    } else {
        g->clearTransform();
        g->setMaterial(m_materialName);
        g->setTransform(m_transform->getTransformMatrix());
        g->drawShape(m_shape);
    }
}
