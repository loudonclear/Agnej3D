#include "PhysicsIntegrator.h"

#include "RigidBody.h"
#include "ShapeCollider.h"
#include "Contact.h"

bool autoSleep = false;
const float epsilon_motion = 2.f;
const float motionBiasCoef = 0.5f;

void PhysicsIntegrator::updateSleepWake(std::shared_ptr<RigidBody> &body, const glm::vec3& deltaPosition, const glm::vec3& deltaRotation, float dt) {
    std::shared_ptr<ShapeCollider> collider = body->getCollider();

    if (autoSleep && body->canSleep) {
		Contact* c;
        bool groundContact = false;
        bool restingContact = false;
        for (Contact *contact : collider->contactInvolvement) {
            if (!contact->isValid()) continue;

            if (!restingContact) {
                restingContact = fabs(contact->desiredDeltaVelocity) <= 0.001f;
				c = contact;
			}

            if (!groundContact) {
                for (int i = 0; i < 2; i++) {
                    if (contact->colliders[i]->isStatic() || !contact->colliders[i]->getRigidBody()->awake) {
                        groundContact = true;
					}
				}
			}

            if (restingContact && groundContact) break;
		}

        const glm::vec3 velocityToConcider = deltaPosition + deltaRotation*15.0f;
        const float currentMotion = glm::dot(velocityToConcider, velocityToConcider);
        const float motionBias = currentMotion > body->motion ? 0 : pow(motionBiasCoef, dt);

        body->motion = glm::clamp(motionBias*body->motion + (1.0f-motionBias)*currentMotion, 0.0f, epsilon_motion*10.0f);

        if (groundContact && currentMotion < epsilon_motion && body->motion < epsilon_motion) {
			body->awake = false;
        }
	} else {
		body->awake = true;
	}
}
