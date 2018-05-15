#include "ShapeCollider.h"

#include "RigidBody.h"
#include "components/Transform.h"
#include "Contact.h"
#include "world/GameObject.h"

#include <algorithm>

ShapeCollider::ShapeCollider(GameObject *parent) : Component(parent), restitution(0.25f), friction(0.85f), transform(nullptr), rigidBody(nullptr) {

}

void ShapeCollider::init() {
    transform = m_gameObject->getComponent<Transform>();
    rigidBody = m_gameObject->getComponent<RigidBody>();
}


glm::vec3 ShapeCollider::_worldToLocal(const glm::vec3 &dirWorldSpace) const {
    return transform->inverseRotateVector(dirWorldSpace);
}

void ShapeCollider::onInvalidate() {
    for (auto& contact : contactInvolvement) {
		contact->invalidate();
	}
}


bool ShapeCollider::isStatic() const {
    return rigidBody == nullptr || rigidBody->isStatic || mass == 0;
}

bool ShapeCollider::isDynamic() const {
	return !isStatic();
}
