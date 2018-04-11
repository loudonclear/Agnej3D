#include "RigidBody.h"
#include "engine/world/GameObject.h"
#include "engine/physics/ShapeCollider.h"

RigidBody::RigidBody(std::shared_ptr<GameObject> &parent, bool isStatic) : Component(parent), m_isStatic(isStatic)
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
