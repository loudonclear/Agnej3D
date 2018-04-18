#include "ShapeCollider.h"
#include <iostream>

ShapeCollider::ShapeCollider(GameObject *parent) : Component(parent), m_transform(nullptr), m_rigidbody(nullptr)
{
    com = glm::vec3(0, 0, 0);
}

void ShapeCollider::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_rigidbody = m_gameObject->getComponent<RigidBody>();
}


glm::vec3 ShapeCollider::getCenterOfMass() {
    return m_transform->transformPoint(com);
}

std::shared_ptr<Transform> ShapeCollider::getTransform() {
    return m_transform;
}

std::shared_ptr<RigidBody> ShapeCollider::getRigidBody() {
    return m_rigidbody;
}

bool ShapeCollider::isStatic() {
    return m_rigidbody ? m_rigidbody->isStatic : true;
}
