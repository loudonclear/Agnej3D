#include "RigidBody.h"
#include "engine/world/GameObject.h"
#include "engine/physics/ShapeCollider.h"

RigidBody::RigidBody(GameObject *parent, bool isStatic, float mass) : Component(parent), m_isStatic(isStatic), m_mass(mass), m_invMass(1.f/mass), isGrounded(false)
{
}

void RigidBody::init() {
    m_transform = m_gameObject->getComponent<Transform>();
}

void RigidBody::onCollide(Collision::ContactData cd) {

    if (cd.s2->isStatic()) {
        m_transform->translate(cd.contactNormal * cd.penetration);
    } else {
        m_transform->translate(cd.contactNormal * cd.penetration / 2.f);
    }
}

void RigidBody::integrateVelocities(float dt) {
    m_transform->translate(dt * velocity);
}

void RigidBody::integrateForces(float dt) {
    if (m_invMass == 0.f) return;

    velocity += (force * dt * m_invMass);
}
