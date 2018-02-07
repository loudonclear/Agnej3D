#include "ShapeCollider.h"

ShapeCollider::ShapeCollider(std::shared_ptr<GameObject> &parent) : Component(parent)
{

}

void ShapeCollider::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_rigidbody = m_gameObject->getComponent<RigidBody>();
}


glm::vec3 ShapeCollider::getCenter() {
    return m_transform->getPosition();
}

std::shared_ptr<Transform> ShapeCollider::getTransform() {
    return m_transform;
}

void ShapeCollider::onCollide(Collision::ContactData cd) {
    if (!isStatic()) {
        m_rigidbody->onCollide(cd);
    }
}

bool ShapeCollider::isColliding() {
    return colliding;
}

void ShapeCollider::setColliding(bool val) {
    colliding = val;
}

bool ShapeCollider::isStatic() {
    return m_rigidbody ? m_rigidbody->isStatic() : true;
}
