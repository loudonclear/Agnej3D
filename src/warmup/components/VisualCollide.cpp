#include "VisualCollide.h"
#include "engine/world/GameObject.h"
#include "engine/components/GraphicsComponent.h"
#include "engine/physics/ShapeCollider.h"

VisualCollide::VisualCollide(GameObject *parent) : Component(parent), TickComponent(parent)
{
}

void VisualCollide::init() {
    m_gc = m_gameObject->getComponent<GraphicsComponent>();
    m_sc = m_gameObject->getComponent<ShapeCollider>();
}

void VisualCollide::tick(float seconds) {
    if (m_sc->isColliding()) {
        m_gc->setMaterial("colliding");
    } else {
        m_gc->setMaterial("notColliding");
    }
}
