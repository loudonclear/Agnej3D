#include "RigidBody.h"
#include "engine/world/GameObject.h"
#include "engine/physics/ShapeCollider.h"

RigidBody::RigidBody(GameObject *parent, bool isStatic, float mass) : Component(parent), isStatic(isStatic), canSleep(true), awake(true), mass(mass), invMass(1.f/mass), motion(0.f),
    velocity(0, 0, 0), angularVelocity(0, 0, 0), force(0, 0, 0), torque(0, 0, 0), lastAcceleration(0, 0, 0), invInertialTensor(1.f)
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
