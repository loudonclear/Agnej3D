#include "EulerIntegrator.h"
#include "RigidBody.h"
#include "ShapeCollider.h"
#include "Contact.h"
#include "components/Transform.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

const glm::vec3 gravity(0, -3, 0);
const float linearDamping = 0.985f;
const float angularDamping = 0.96f;

void EulerIntegrator::integrateBodies(std::vector<std::shared_ptr<RigidBody>> &bodies, float dt) {

    for (std::shared_ptr<RigidBody> &body : bodies) {

        const std::shared_ptr<ShapeCollider> &collider = body->getCollider();
        if (collider == nullptr || collider->isStatic() || !body->awake) continue;

        body->acceleration = gravity + body->force * collider->inverseMass;
        body->acceleration *= dt;

        const glm::vec3 acceleration = body->acceleration + body->force * collider->inverseMass;
        const glm::vec3 angularAcceleration = Transform::transformPoint(body->torque, collider->invInertialTensor);

        body->velocity += acceleration;
        body->angularVelocity += angularAcceleration;

        body->velocity *= pow(linearDamping, dt);
        body->angularVelocity *= pow(angularDamping, dt);

        glm::vec3 adjustedVelocity = body->velocity;
        for (Contact* contact : collider->contactInvolvement) {
            if (!contact->isValid()) continue;

            const int otherColliderInd = collider == contact->colliders[0] ? 0 : 1;
            const float sign = float(otherColliderInd == 0 ? -1 : 1);

            if (contact->penetration >= 0) {
                adjustedVelocity -= sign * std::max(sign*glm::dot(adjustedVelocity, contact->normal), 0.0f) * contact->normal;
            }
        }

        body->getTransform()->translate(adjustedVelocity * dt);
        body->getTransform()->rotateAngularAmt(body->angularVelocity * dt);

        body->force = glm::vec3(0.f);
        body->torque = glm::vec3(0.f);

        updateSleepWake(body,adjustedVelocity,body->angularVelocity, dt);
	}
}
