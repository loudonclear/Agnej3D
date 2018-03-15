#include "RigidBody.h"

RigidBody::RigidBody(std::shared_ptr<Transform> &t, float mass) : m_transform(t), m_mass(mass), m_invMass(1.f/mass)
{
}

std::shared_ptr<Transform> RigidBody::getTransformFuture(float seconds) {
    std::shared_ptr<Transform> res = std::make_shared<Transform>();
    res->setPosition(m_transform->getPosition() + seconds*velocity);
    return res;
}
