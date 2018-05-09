#include "Contact.h"

#include "ShapeCollider.h"
#include "RigidBody.h"
#include "components/Transform.h"

#include <algorithm>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

Contact::Contact(const ContactBasicData& data) : ContactBasicData(data) {
	invalidFlag = false;

    colliderIterators[0] = colliders[0]->contactInvolvement.insert(this);
    colliderIterators[1] = colliders[1]->contactInvolvement.insert(this);

	initialContactPoint = point;
}

Contact::Contact(Contact&& other) {
	invalidFlag = other.invalidFlag;
	other.invalidFlag = true;

	point = other.point;
	normal = other.normal;
	penetration = other.penetration;
	colliders[0] = other.colliders[0]; other.colliders[0] = nullptr;
	colliders[1] = other.colliders[1]; other.colliders[1] = nullptr;

	initialContactPoint = other.initialContactPoint;

	matContactToWorld = other.matContactToWorld;
	matWorldToContact = other.matWorldToContact;
	closingVelocity_world = other.closingVelocity_world;
	closingVelocity_contact = other.closingVelocity_contact;
	relativeContactPosition[0] = other.relativeContactPosition[0];
	relativeContactPosition[1] = other.relativeContactPosition[1];
	coefRestitution = other.coefRestitution;
	coefFriction = other.coefFriction;

    desiredDeltaVelocity = other.desiredDeltaVelocity;

    colliderIterators[0] = other.colliderIterators[0];
    colliderIterators[1] = other.colliderIterators[1];

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
			triangleSupports_local[i][j] = other.triangleSupports_local[i][j];

    for (int i = 0; i < 2; i++) {
        if (colliders[i] != nullptr) {
            *colliderIterators[i] = this;
		}
	}

	other.colliders[0] = other.colliders[1] = {};
    other.colliderIterators[0] = other.colliderIterators[1] = {};
}

Contact::~Contact() {
    for (int i = 0; i < 2; i++) {
        if (colliders[i] != nullptr) {
            colliders[i]->contactInvolvement.remove(colliderIterators[i]);
		}
	}
}

bool Contact::isValid() const {
    if (invalidFlag) return false;

    for (int i = 0; i < 2; i++) {
        if (colliders[i] == nullptr) return false;
	}

	return true;
}

void Contact::invalidate() {
	invalidFlag = true;
}

void makeOrthoNormalBasis(glm::vec3 &x, glm::vec3 &y, glm::vec3 &z) {
    x = glm::normalize(x);
    z = glm::cross(x, (abs(x.x) > abs(x.y) ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0)));
    if (glm::length2(z) == 0.f) return;
    z = glm::normalize(z);
    y = glm::cross(z, x);
}

glm::mat3x3 orthoNormalize(glm::mat3x3 &m) {
    glm::vec3 right =   glm::vec3(m[0][0], m[0][1], m[0][2]);
    glm::vec3 up =      glm::vec3(m[1][0], m[1][1], m[1][2]);
    glm::vec3 forward = glm::vec3(m[2][0], m[2][1], m[2][2]);

    makeOrthoNormalBasis(right, up, forward);

    return glm::mat3x3(right.x, right.y, right.z,
                       up.x, up.y, up.z,
                       forward.x, forward.y, forward.z);
}

void Contact::calcData() {
    glm::mat3x3 temp = glm::mat3x3(1.f);
    temp[0][0] = normal.x;
    temp[0][1] = normal.y;
    temp[0][2] = normal.z;
    matContactToWorld = glm::mat4x4(orthoNormalize(temp));
    matWorldToContact = glm::transpose(matContactToWorld);

    const float restitutionLow = (std::min)(colliders[0]->restitution,colliders[1]->restitution);
    const float restitutionHigh = (std::max)(colliders[0]->restitution,colliders[1]->restitution);

    const float frictionLow = (std::min)(colliders[0]->friction,colliders[1]->friction);
    const float frictionHigh = (std::max)(colliders[0]->friction,colliders[1]->friction);

    const float targetRestitution = glm::mix(restitutionLow,restitutionHigh,0.15f);
    const float targetFriction = glm::mix(frictionLow,frictionHigh,0.15f);

    closingVelocity_world = glm::vec3(0.f);
    closingVelocity_contact = glm::vec3(0.f);

    for (int i = 0; i < 2; i++) {
        relativeContactPosition[i] = point - colliders[i]->getCenterOfMass();
		
        const std::shared_ptr<RigidBody> &rigidBody = colliders[i]->getRigidBody();
        if (rigidBody == nullptr) return;

        const glm::vec3& planarAcceleration = rigidBody->acceleration - (glm::dot(rigidBody->acceleration, normal) * normal);
        const glm::vec3& localVelocity_world = rigidBody->velocity + planarAcceleration + glm::cross(rigidBody->angularVelocity, relativeContactPosition[i]);

		const float sign = (i == 0 ? -1.0f : 1.0f);

		closingVelocity_world += sign * localVelocity_world;
	}

    closingVelocity_contact = Transform::transformVector(closingVelocity_world, matWorldToContact);

	coefRestitution = targetRestitution;
	coefFriction = targetFriction;
}
