#include "RigidBody.h"
#include "engine/world/GameObject.h"
#include "engine/physics/ShapeCollider.h"

RigidBody::RigidBody(GameObject *parent, float mass) : Component(parent), isStatic(isStatic), mass(mass), invMass(1.f/mass)
{
}

void RigidBody::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_collider = m_gameObject->getComponent<ShapeCollider>();
}

void RigidBody::addForceAtPoint(const glm::vec3 &f, const glm::vec3 &p) {
    const glm::vec3 ctp = p - m_transform->getPosition();

    force += f;
    torque += glm::cross(ctp, f);
}
