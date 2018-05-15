#include "ImpulseContactResolver.h"

#include "RigidBody.h"
#include "ShapeCollider.h"
#include "Contact.h"
#include "components/Transform.h"

#include <algorithm>
#include <iostream>
#include <glm/gtx/string_cast.hpp>


const float minimumVelocityForRestitution = 2.5f;
const float epsilon_penetration = 0.01f;
const float epsilon_velocity = 0.01f;

const int iterations = 10;

float ImpulseContactResolver::deltaVelocity(Contact* contact) {
	float velocityFromAcceleration = 0;

    for (int i = 0; i < 2; i++) {
        const std::shared_ptr<RigidBody> rigidBody = contact->colliders[i]->getRigidBody();
        const glm::vec3& acceleration = (rigidBody != nullptr ? (rigidBody->acceleration) : (glm::vec3(0.f)));

		const float sign = (i == 0 ? -1.0f : 1.0f);
        velocityFromAcceleration += sign * glm::dot(acceleration, contact->normal);
	}

    const float effectiveRestitution = contact->closingVelocity_contact.x - velocityFromAcceleration >= minimumVelocityForRestitution ? contact->coefRestitution : 0.0f;
	return -contact->closingVelocity_contact.x - (effectiveRestitution * (contact->closingVelocity_contact.x - velocityFromAcceleration));
}

glm::vec3 ImpulseContactResolver::calculateImpulse(Contact* contact) {
	float totalMass_inverse = 0;
    glm::mat4x4 rotationalInertia_inv_world(0.f);

    for (int i = 0; i < 2; i++) {
        if (contact->colliders[i]->inverseMass == 0) continue;

        glm::vec3 v = contact->relativeContactPosition[i];
        const glm::mat4x4 impulseToTorque = glm::mat4x4(0, -v.z, v.y, 0,
                                                        v.z, 0, -v.x, 0,
                                                        -v.y, v.x, 0, 0,
                                                        0, 0, 0, 1);
        glm::mat4x4 myRotationalInertia_inv_local = -1.0f * impulseToTorque * contact->colliders[i]->invInertialTensor * impulseToTorque;

		rotationalInertia_inv_world += myRotationalInertia_inv_local;
		totalMass_inverse += contact->colliders[i]->inverseMass;
	}
    rotationalInertia_inv_world[3][3] = -1;

    glm::mat4x4 totalInertia_inv_contact = contact->matWorldToContact * rotationalInertia_inv_world * contact->matContactToWorld;
    totalInertia_inv_contact[0][0] += totalMass_inverse;
    totalInertia_inv_contact[1][1] += totalMass_inverse;
    totalInertia_inv_contact[2][2] += totalMass_inverse;

    const glm::mat4x4 impulsePerUnitVelocity = glm::inverse(totalInertia_inv_contact);

    const glm::vec3 velocityToKill(contact->desiredDeltaVelocity,
								 -contact->closingVelocity_contact.y,
								 -contact->closingVelocity_contact.z);

    glm::vec3 impulse_contact = Transform::transformPoint(velocityToKill, impulsePerUnitVelocity);

    // Tangential friction in both perpendicular directions
    const float planarImpulse = sqrt(impulse_contact.y*impulse_contact.y + impulse_contact.z*impulse_contact.z);
    if (planarImpulse > impulse_contact.x * contact->coefFriction) { // Dynamic friction
		impulse_contact.y /= planarImpulse;
		impulse_contact.z /= planarImpulse;
		impulse_contact.x =
            totalInertia_inv_contact[0][0] +
            totalInertia_inv_contact[0][1] * contact->coefFriction * impulse_contact.y +
            totalInertia_inv_contact[0][2] * contact->coefFriction * impulse_contact.z;

        impulse_contact.x = contact->desiredDeltaVelocity / impulse_contact.x;
		impulse_contact.y *= contact->coefFriction * impulse_contact.x;
		impulse_contact.z *= contact->coefFriction * impulse_contact.x;

		impulse_contact *= -1;
	}

    return Transform::transformPoint(impulse_contact, contact->matContactToWorld);
}

void ImpulseContactResolver::resolveContactCluster(std::list<Contact*> &cluster) {

    int posIter = 0;
    int velIter = 0;

    for (Contact* contact : cluster) {
        for (int i = 0; i < 2; i++) {
            auto rigidBody = contact->colliders[i]->getRigidBody();
            if (rigidBody != nullptr) rigidBody->awake = true;
		}

        contact->calcData();
        contact->desiredDeltaVelocity = deltaVelocity(contact);
	}

    // Positional correction
    for (posIter = 0; posIter < iterations; posIter++) {
        Contact* contactHighestPenetration = nullptr;
        for (auto &contact : cluster) {
            if (contactHighestPenetration == nullptr || contact->penetration > contactHighestPenetration->penetration) {
                contactHighestPenetration = contact;
			}
		}

        if (contactHighestPenetration == nullptr || contactHighestPenetration->penetration <= epsilon_penetration) {
            posIter++;
			break;
		}

        float totalInertia = 0;
        float linearInertia[2] = {0, 0};
        float angularInertia[2] = {0, 0};

        // Normal inertia
        for (int i = 0; i < 2; i++) {
            auto rigidBody = contactHighestPenetration->colliders[i]->getRigidBody();
            auto collider = contactHighestPenetration->colliders[i];
            if (rigidBody == nullptr || collider->isStatic()) continue;

            glm::vec3 angularInertiaWorld = glm::cross(contactHighestPenetration->relativeContactPosition[i], contactHighestPenetration->normal);
            angularInertiaWorld = Transform::transformPoint(angularInertiaWorld, collider->invInertialTensor);
            angularInertiaWorld = glm::cross(angularInertiaWorld, contactHighestPenetration->relativeContactPosition[i]);
            angularInertia[i] = glm::dot(angularInertiaWorld, contactHighestPenetration->normal);

            linearInertia[i] = contactHighestPenetration->colliders[i]->inverseMass;

            totalInertia += linearInertia[i] + angularInertia[i];
        }

        for (int i = 0; i < 2; i++) {
            auto collider = contactHighestPenetration->colliders[i];
            if (collider->isStatic()) continue;

            auto rigidBody = collider->getRigidBody();

            const float sign = (float)(i == 0 ? 1 : -1);
            float linearMovementMagnitude = sign * contactHighestPenetration->penetration * (linearInertia[i] / totalInertia);
            float angularMovementMagnitude = sign * contactHighestPenetration->penetration * (angularInertia[i] / totalInertia);

            const glm::vec3 relativeContactPosition =
                contactHighestPenetration->relativeContactPosition[i] +
                (contactHighestPenetration->normal * -glm::dot(contactHighestPenetration->relativeContactPosition[i], contactHighestPenetration->normal));

            float angularMovementLimit = 0.2f * glm::length(relativeContactPosition);

            if (abs(angularMovementMagnitude) > angularMovementLimit) {
                float totalMove = linearMovementMagnitude + angularMovementMagnitude;
                angularMovementMagnitude = glm::clamp(angularMovementMagnitude,-angularMovementLimit,angularMovementLimit);
                linearMovementMagnitude = totalMove - angularMovementMagnitude;
            }

            glm::vec3 linearDelta = contactHighestPenetration->normal * linearMovementMagnitude;
            glm::vec3 angularDelta = glm::vec3(0.f);
            if (angularMovementMagnitude != 0) {
                const glm::vec3 rotationDirection = glm::cross(contactHighestPenetration->relativeContactPosition[i], contactHighestPenetration->normal);
                angularDelta = Transform::transformPoint(rotationDirection, collider->invInertialTensor) * (angularMovementMagnitude / angularInertia[i]);
            }

            rigidBody->getTransform()->translate(linearDelta);
            rigidBody->getTransform()->rotateAngularAmt(angularDelta);


            for (Contact *contact : collider->contactInvolvement) {
                if (!contact->isValid()) continue;

                const int colliderIndInOtherContact = contact->colliders[0] == collider ? 0 : 1;
                const float sign = float(colliderIndInOtherContact == 0 ? -1 : 1);

                const glm::vec3 deltaPosition = (linearDelta + glm::cross(angularDelta, contact->relativeContactPosition[colliderIndInOtherContact]));
                contact->penetration += sign * glm::dot(deltaPosition, contact->normal);

                contact->relativeContactPosition[1] += deltaPosition * sign;

                if (colliderIndInOtherContact == 0) {
                    contact->point += deltaPosition;
                }
            }
        }
	}

    for (velIter = 0; velIter < iterations; velIter++) {

        Contact* contactHighestDeltaVelocity = nullptr;
        for (Contact *contact : cluster) {
            if (contact->penetration < -0.01f) continue;

            if (contactHighestDeltaVelocity == nullptr || contact->desiredDeltaVelocity < contactHighestDeltaVelocity->desiredDeltaVelocity) {
                contactHighestDeltaVelocity = contact;
			}
		}

        if (contactHighestDeltaVelocity == nullptr || contactHighestDeltaVelocity->desiredDeltaVelocity >= -epsilon_velocity) {
            velIter++;
			break;
		}

        const glm::vec3 impulse = calculateImpulse(contactHighestDeltaVelocity);

        for (int i = 0; i < 2; i++) {
            auto collider = contactHighestDeltaVelocity->colliders[i];
            if (collider->isStatic()) continue;

            auto rigidBody = collider->getRigidBody();

			const float sign = (i == 0 ? 1.0f : -1.0f);

            const glm::vec3 velDelta = sign * (impulse * collider->inverseMass);
			rigidBody->velocity += velDelta;

            const glm::vec3 avelDelta = sign * Transform::transformPoint(glm::cross(contactHighestDeltaVelocity->relativeContactPosition[i], impulse), collider->invInertialTensor);
			rigidBody->angularVelocity += avelDelta;

            for (Contact* contact : collider->contactInvolvement) {
                if (!contact->isValid()) continue;

                const int colliderIndInOtherContact = contact->colliders[0] == collider ? 0 : 1;
                const float sign = float(colliderIndInOtherContact == 0 ? -1 : 1);

                const glm::vec3 delta = velDelta + glm::cross(avelDelta, contact->relativeContactPosition[colliderIndInOtherContact]);
				contact->closingVelocity_world += sign * delta;
                contact->closingVelocity_contact = Transform::transformPoint(contact->closingVelocity_world, contact->matWorldToContact);
                contact->desiredDeltaVelocity = deltaVelocity(contact);
			}
		}
	}
}

void ImpulseContactResolver::buildContactClusters(pool_vector<Contact> &contacts, std::list<std::list<Contact*>> &contactClusters) {

    for (Contact& contact : contacts) {
        if (!contact.isValid()) continue;

        for (int i = 0; i < 2; i++) {
            contact.colliders[i]->cluster = nullptr;
		}
	}

    for (Contact& contact : contacts) {
        if (!contact.isValid()) continue;

        for (int i = 0; i < 2; i++) {
            if (contact.colliders[i]->cluster == nullptr && contact.colliders[i]->isDynamic()) {
				contactClusters.emplace_back();
                contact.colliders[i]->cluster = &contactClusters.back();
			}
		}

        std::shared_ptr<ShapeCollider> const colliderA = contact.colliders[0];
        std::shared_ptr<ShapeCollider> const colliderB = contact.colliders[1];

        std::list<Contact*>* clusterA = reinterpret_cast<std::list<Contact*>*>(colliderA->cluster);
        std::list<Contact*>* clusterB = reinterpret_cast<std::list<Contact*>*>(colliderB->cluster);

        if (clusterA == nullptr || clusterB == nullptr) {
            if (clusterA != nullptr) clusterA->push_back(&contact);
            if (clusterB != nullptr) clusterB->push_back(&contact);
		} else {
            if (clusterA != clusterB) { // merge
				clusterA->splice(clusterA->end(),*clusterB);
                colliderB->cluster = clusterB = clusterA;
			}
			clusterA->push_back(&contact);
		}
	}

    contactClusters.remove_if([](const std::list<Contact*> &item)->bool {
		return item.size() == 0;
	});
}

void ImpulseContactResolver::frame(pool_vector<Contact> &contacts) {

    std::list<std::list<Contact*>> contactClusters;
    buildContactClusters(contacts, contactClusters);

    for (auto &cluster : contactClusters) {
        resolveContactCluster(cluster);
	}
}
