#include "RigidBody.h"

#include "ShapeCollider.h"
#include "components/Transform.h"
#include "world/GameObject.h"

RigidBody::RigidBody(GameObject *parent, float density) : Component(parent), density(density), awake(true), motion(0.f), isStatic(false), canSleep(true), interaction(false), transform(nullptr), collider(nullptr) {

}

void RigidBody::init() {
    transform = m_gameObject->getComponent<Transform>();
    collider = m_gameObject->getComponent<ShapeCollider>();
}


void RigidBody::addForce(const glm::vec3 &f) {
    this->force += f;
}

void RigidBody::addForceWorld(const glm::vec3 &f, const glm::vec3 &p) {
    this->force += f;
    torque += glm::cross(p - transform->getPosition(), force);
}

void RigidBody::addTorque(const glm::vec3 &t) {
    torque += t;
}

