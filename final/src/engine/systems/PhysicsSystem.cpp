#include "PhysicsSystem.h"
#include "engine/physics/RigidBody.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/Collision.h"
#include "engine/physics/Contact.h"

#include <glm/gtx/string_cast.hpp>

#include <algorithm>
#include <iostream>

const glm::vec3 gravity(0, -1, 0);
const float linearDamping = 0.785f;
const float angularDamping = 0.756f;
const bool autoSleep = true;
const float epsilonMotion = 2.0f;
const float motionBiasCoef = 0.5f;
//const float maxPhysicsDist = 10000 * 10000;
const float minPenetration = -8.0f;

PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<RigidBody> component = go->getComponent<RigidBody>();
    if (component != nullptr) m_components.push_back(component);
}

void PhysicsSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<RigidBody> component = go->getComponent<RigidBody>();
    if (component != nullptr)
        m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void PhysicsSystem::update(float dt) {
    if (m_components.size() < 1) return;

    // Integration
    integrate(dt);

    // Contact generation
    generateContacts();

    auto iter = contacts.begin();
    for ( ; iter != contacts.end(); ) {
        if (!iter->isValid()) {
            iter = contacts.erase(iter);
        } else {
            ++iter;
        }
    }

    // Resolve contacts
    resolveContacts();
}

void PhysicsSystem::integrate(float dt) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        std::shared_ptr<RigidBody> rb = *iter;
        std::shared_ptr<ShapeCollider> sc = rb->getShapeCollider();
        std::shared_ptr<Transform> t = sc->getTransform();

        if (rb->invMass != 0 && !rb->isStatic && rb->awake) {
            rb->lastAcceleration = dt * (gravity + rb->force * rb->invMass);

            const glm::vec3 acceleration = rb->lastAcceleration + rb->force * rb->invMass;
            const glm::vec3 angularAcceleration = rb->torque * glm::mat3x3(rb->invInertialTensor);

            rb->velocity += acceleration;
            rb->angularVelocity += angularAcceleration;

            rb->velocity *= pow(linearDamping, dt);
            rb->angularVelocity *= pow(angularDamping, dt);

            glm::vec3 adjVel = rb->velocity;

            for (auto &contact : sc->contacts) {
                if (!contact->isValid()) continue;

                if (contact->penetration >= 0) {
                    const float sign = (sc == contact->c[0]) ? 1.f : -1.f;
                    adjVel += sign * std::max(-sign*glm::dot(adjVel, contact->normal), 0.f) * contact->normal;
                }
            }

            t->translate(adjVel * dt);
            glm::quat eu = glm::quat(-rb->angularVelocity * dt);
            eu = t->getRotation() * eu;
            t->setRotation(glm::normalize(t->getRotation() + 0.5f * eu));

            rb->force = glm::vec3(0, 0, 0);
            rb->torque = glm::vec3(0, 0, 0);

        }
    }
}

void PhysicsSystem::updateSleepWake(std::shared_ptr<RigidBody> &rb, std::shared_ptr<ShapeCollider> &sc, const glm::vec3 &dp, const glm::vec3 &dr, float dt) {

    if (autoSleep && rb->canSleep) {
        Contact *c;
        bool grounded = false;
        bool restingContact = false;

        for (auto &contact : sc->contacts) {
            if (!contact->isValid()) continue;

            if (!restingContact) {
                restingContact = fabs(contact->desiredDeltaVelocity) <= FLOAT_EPSILON;
                c = contact;
            }

            if (!grounded) {
                if (contact->c[0]->isStatic() || !contact->c[0]->getRigidBody()->awake
                 || contact->c[1]->isStatic() || !contact->c[1]->getRigidBody()->awake) {
                    grounded = true;
                }
            }

            if (grounded && restingContact) break;
        }

        const glm::vec3 velConsider = dp + 15.f * dr;
        const float currentMotion = glm::dot(velConsider, velConsider);
        const float motionBias = currentMotion > rb->motion ? 0 : pow(motionBiasCoef, dt);

        rb->motion = glm::clamp(rb->motion + (1.f - motionBias) * currentMotion, 0.0f, epsilonMotion * 10.f);

        if (grounded && currentMotion < epsilonMotion && rb->motion < epsilonMotion) {
            rb->awake = false;
        }

    } else {
        rb->awake = true;
    }
}

void PhysicsSystem::generateContacts() {

    // Update Persistent Contacts
    for (Contact &c : contacts) {
        if (c.isValid()) {
            if ((c.c[0]->isStatic() || !c.c[0]->getRigidBody()->awake) && (c.c[1]->isStatic() || !c.c[1]->getRigidBody()->awake)) {
                c.invalidFlag = true;
            } else {
                if (c.penetration < minPenetration) {
                    c.invalidFlag = true;
                }
            }
        }
    }

    // Generate new contacts
    std::vector<Contact> newContacts;
    for (unsigned int i = 0; i < m_components.size() - 1; i++) {
        std::shared_ptr<RigidBody> rb1 = m_components.at(i);

        for (unsigned int j = 1; j < m_components.size(); j++) {
            std::shared_ptr<RigidBody> rb2 = m_components.at(j);

            if (!(!rb1->isStatic && rb1->awake) && !(!rb2->isStatic && rb2->awake)) {
                continue;
            }

            Contact c;
            c.c[0] = rb1->getShapeCollider();
            c.c[1] = rb2->getShapeCollider();
            int uniqueId;
            Collision::Simplex s;
            if (Collision::gjk(rb1->getShapeCollider(), rb2->getShapeCollider(), s, &uniqueId) && Collision::epa(rb1->getShapeCollider(), rb2->getShapeCollider(), s, &uniqueId, &c)) {
                //if (glm::dot(rb1->velocity, c.normal) >= 0.f && glm::dot(rb2->velocity, c.normal) <= 0.f) continue;
                newContacts.push_back(c);
            }
        }
    }

    // Add new contacts removing similar ones
    for (Contact &c : newContacts) {
        const std::shared_ptr<ShapeCollider> scCheck = c.c[0]->contacts.size() < c.c[1]->contacts.size() ? c.c[0] : c.c[1];

        for (Contact *oc : scCheck->contacts) {
            if (!oc->isValid()) continue;

            if ((c.c[0] == oc->c[0] && c.c[1] == oc->c[1]) || (c.c[0] == oc->c[1] && c.c[1] == oc->c[0])) {
                if (glm::dot(c.normal, oc->normal) > 0.95f) {
                    if (glm::length(c.point - oc->point) < 5.f) {
                        oc->invalidFlag = true;
                        break;
                    }
                }
            }
        }

        contacts.emplace_back(std::move(c));
    }
}

void PhysicsSystem::resolveContacts() {

    std::list<std::list<Contact*>> contactClusters;

    for (Contact &c : contacts) {
        if (!c.isValid()) continue;

        c.c[0]->contactCluster = nullptr;
        c.c[1]->contactCluster = nullptr;
    }

    for (Contact &c : contacts) {
        if (!c.isValid()) continue;

        if (c.c[0]->contactCluster == nullptr && !c.c[0]->isStatic()) {
            contactClusters.emplace_back();
            c.c[0]->contactCluster = &contactClusters.back();
        }
        if (c.c[1]->contactCluster == nullptr && !c.c[1]->isStatic()) {
            contactClusters.emplace_back();
            c.c[1]->contactCluster = &contactClusters.back();
        }


        std::list<Contact*>* clusterA = reinterpret_cast<std::list<Contact*>*>(c.c[0]->contactCluster);
        std::list<Contact*>* clusterB = reinterpret_cast<std::list<Contact*>*>(c.c[1]->contactCluster);

        if (clusterA == nullptr || clusterB == nullptr) {
            if (clusterA != nullptr) clusterA->push_back(&c);
            if (clusterB != nullptr) clusterB->push_back(&c);
        } else {
            if (clusterA != clusterB) {
                clusterA->splice(clusterA->end(),*clusterB);
                c.c[1]->contactCluster = clusterB = clusterA;
            }
            clusterA->push_back(&c);
        }
    }

    contactClusters.remove_if([](const std::list<Contact*> &item)->bool {
        return item.size() == 0;
    });

    for (auto &cluster : contactClusters) {
        resolveCluster(cluster);
    }
}

void PhysicsSystem::resolveCluster(std::list<Contact*> &cluster) {

    int maxPenetrationIterations = 5 + round(1.5f * cluster.size());
    int maxVelocityIterations = 5 + round(1.5f * cluster.size());

    int numPenetrationIters = 0;
    int numVelocityIters = 0;

    for (auto &c : cluster) {
        c->c[0]->getRigidBody()->awake = true;
        c->c[1]->getRigidBody()->awake = true;

        c->calculateData();
        c->desiredDeltaVelocity = calcDesiredDeltaVelocity(c);
    }

    const float penetrationPercentFix = 0.35f;
    for (numPenetrationIters = 0; numPenetrationIters < maxPenetrationIterations; numPenetrationIters++) {
        Contact *highestPenetration = nullptr;
        for (auto &c : cluster) {
            if (highestPenetration == nullptr || c->penetration > highestPenetration->penetration) {
                highestPenetration = c;
            }
        }

        if (highestPenetration == nullptr || highestPenetration->penetration <= 0.01f) {
            numPenetrationIters++;
            break;
        }

        ; {
            float totalInertia = 0;
            float linearInertia[2] = {0,0};
            float angularInertia[2] = {0,0};

            for(int i = 0; i < 2; i++) {
                if (highestPenetration->c[i]->isStatic()) continue;

                glm::vec3 angularInertiaWorld = glm::cross(highestPenetration->relativeContactPosition[i], highestPenetration->normal);
                angularInertiaWorld = angularInertiaWorld * glm::mat3x3(highestPenetration->c[i]->getRigidBody()->invInertialTensor);
                angularInertiaWorld = glm::cross(angularInertiaWorld, highestPenetration->relativeContactPosition[i]);
                angularInertia[i] = glm::dot(angularInertiaWorld, highestPenetration->normal);

                linearInertia[i] = highestPenetration->c[i]->getRigidBody()->invMass;

                totalInertia += linearInertia[i] + angularInertia[i];
            }

            for (int i = 0; i < 2; i++) {
                auto collider = highestPenetration->c[i];
                if (collider->isStatic()) continue;

                //auto rigidBody = collider->getRigidBody();

                const float sign = (float)(i == 0 ? 1 : -1);
                float linearMovementMagnitude = penetrationPercentFix * sign * highestPenetration->penetration * (linearInertia[i] / totalInertia);
                float angularMovementMagnitude = penetrationPercentFix * sign * highestPenetration->penetration * (angularInertia[i] / totalInertia);

                const glm::vec3 relativeContactPositionInNormalDirection =
                    highestPenetration->relativeContactPosition[i] +
                    (highestPenetration->normal * -(glm::dot(highestPenetration->relativeContactPosition[i], highestPenetration->normal)));

                float angularMovementLimit = 0.2f * glm::length(relativeContactPositionInNormalDirection);

                if(abs(angularMovementMagnitude) > angularMovementLimit) {
                    float totalMove = linearMovementMagnitude + angularMovementMagnitude;
                    angularMovementMagnitude = glm::clamp(angularMovementMagnitude,-angularMovementLimit,angularMovementLimit);
                    linearMovementMagnitude = totalMove - angularMovementMagnitude;
                }

                glm::vec3 linearDelta = highestPenetration->normal * linearMovementMagnitude;
                glm::vec3 angularDelta = glm::vec3(0, 0, 0);
                if (angularMovementMagnitude != 0) {
                    const glm::vec3 rotationDirection = glm::cross(highestPenetration->relativeContactPosition[i], highestPenetration->normal);
                    angularDelta = (rotationDirection * glm::mat3x3(collider->getRigidBody()->invInertialTensor)) * (angularMovementMagnitude / angularInertia[i]);
                }

                collider->getTransform()->translate(linearDelta);
                glm::quat eu = glm::quat(-angularDelta);
                eu = collider->getTransform()->getRotation() * eu;
                collider->getTransform()->setRotation(glm::normalize(collider->getTransform()->getRotation() + 0.5f * eu));

                for (auto &contact : collider->contacts) {
                    if (!contact->isValid()) continue;

                    const int colliderInd = contact->c[0] == collider ? 0 : 1;
                    const float sign = float(colliderInd == 0 ? -1 : 1);

                    const glm::vec3 deltaPosition = (linearDelta + (glm::cross(angularDelta, contact->relativeContactPosition[colliderInd])));
                    contact->penetration += sign * (glm::dot(deltaPosition, contact->normal));

                    contact->relativeContactPosition[1] += deltaPosition * sign;

                    if (colliderInd == 0) {
                        contact->point += deltaPosition;
                    }
                }
            }
        }
    }

    for(numVelocityIters = 0; numVelocityIters < maxVelocityIterations; numVelocityIters++) {

        Contact* contactWithHighestDesiredDeltaVelocity = nullptr;
        for (auto& contact : cluster) {
            if (contact->penetration < -0.1f) continue;

            if (contactWithHighestDesiredDeltaVelocity == nullptr || contact->desiredDeltaVelocity < contactWithHighestDesiredDeltaVelocity->desiredDeltaVelocity) {
                contactWithHighestDesiredDeltaVelocity = contact;
            }
        }

        if(contactWithHighestDesiredDeltaVelocity == nullptr || contactWithHighestDesiredDeltaVelocity->desiredDeltaVelocity >= -0.025f) {
            numVelocityIters++;
            break;
        }

        const glm::vec3 impulse = calculateImpulse(contactWithHighestDesiredDeltaVelocity);

        for (int i = 0; i < 2; i++) {
            auto collider = contactWithHighestDesiredDeltaVelocity->c[i];
            if (collider->isStatic()) continue;

            auto rigidBody = collider->getRigidBody();

            const float sign = (i == 0 ? 1.0f : -1.0f);

            const glm::vec3 velDelta = sign * (impulse * rigidBody->invMass);
            rigidBody->velocity += velDelta;

            const glm::vec3 avelDelta = sign * ((glm::cross(contactWithHighestDesiredDeltaVelocity->relativeContactPosition[i], impulse)) * glm::mat3x3(collider->getRigidBody()->invInertialTensor));
            rigidBody->angularVelocity += avelDelta;

            for(auto& contact : collider->contacts) {
                if(!contact->isValid()) continue;

                const int colliderIndInOtherContact = contact->c[0] == collider ? 0 : 1;
                const float sign = float(colliderIndInOtherContact==0 ? -1 : 1);

                const glm::vec3 delta = velDelta + glm::cross(avelDelta, contact->relativeContactPosition[colliderIndInOtherContact]);
                contact->closingVelocity_world += sign * delta;
                contact->closingVelocity_contact = contact->closingVelocity_world * glm::mat3x3(contact->matWorldToContact);
                contact->desiredDeltaVelocity = calcDesiredDeltaVelocity(contact);
            }
        }
    }

}

float PhysicsSystem::calcDesiredDeltaVelocity(Contact *c) {
    float velocityFromAcceleration = 0;

    velocityFromAcceleration -= glm::dot(c->c[0]->getRigidBody()->lastAcceleration, c->normal);
    velocityFromAcceleration += glm::dot(c->c[1]->getRigidBody()->lastAcceleration, c->normal);

    const float effectiveRestitution = c->closingVelocity_contact.x - velocityFromAcceleration >= 2.5f ? c->coefRestitution : 0.0f;

    return -c->closingVelocity_contact.x - (effectiveRestitution * (c->closingVelocity_contact.x - velocityFromAcceleration));
}

glm::vec3 PhysicsSystem::calculateImpulse(Contact* contact) {
    float totalMass_inverse = 0;
    glm::mat4x4 rotationalInertia_inv_world(0.f);

    for (int i = 0; i < 2; i++) {
        if(contact->c[i]->getRigidBody()->invMass == 0) continue;

        const glm::vec3 v = contact->relativeContactPosition[i];
        const glm::mat4x4 impulseToTorque = glm::transpose(glm::mat4x4(0, -v.z, v.y, 0,
                                                                       v.z, 0, -v.x, 0,
                                                                       -v.y, v.x, 0, 0,
                                                                       0, 0, 0, 1));
        glm::mat4x4 myRotationalInertia_inv_local = -1.0f * impulseToTorque * contact->c[i]->getRigidBody()->invInertialTensor * impulseToTorque;

        rotationalInertia_inv_world += myRotationalInertia_inv_local;

        totalMass_inverse += contact->c[i]->getRigidBody()->invMass;
    }

    rotationalInertia_inv_world[3][3] = -1;

    glm::mat4x4 totalInertia_inv_contact = contact->matContactToWorld * rotationalInertia_inv_world * contact->matWorldToContact;
    totalInertia_inv_contact[0][0] += totalMass_inverse;
    totalInertia_inv_contact[1][1] += totalMass_inverse;
    totalInertia_inv_contact[2][2] += totalMass_inverse;

    const glm::mat4x4 impulsePerUnitVelocity = glm::inverse(totalInertia_inv_contact);

    const glm::vec3 velocityToKill(contact->desiredDeltaVelocity,
                                 -contact->closingVelocity_contact.y,
                                 -contact->closingVelocity_contact.z);

    glm::vec3 impulse_contact = velocityToKill * glm::mat3x3(impulsePerUnitVelocity);

    const float planarImpulse = sqrt((impulse_contact.y * impulse_contact.y) + (impulse_contact.z * impulse_contact.z));
    if (planarImpulse > impulse_contact.x * contact->coefFriction) {
        impulse_contact.y /= planarImpulse;
        impulse_contact.z /= planarImpulse;
        impulse_contact.x =
            totalInertia_inv_contact[0][0] +
            totalInertia_inv_contact[1][0] * contact->coefFriction * impulse_contact.y +
            totalInertia_inv_contact[2][0] * contact->coefFriction * impulse_contact.z;

        impulse_contact.x = contact->desiredDeltaVelocity / impulse_contact.x;
        impulse_contact.y *= contact->coefFriction * impulse_contact.x;
        impulse_contact.z *= contact->coefFriction * impulse_contact.x;

        impulse_contact *= -1;
    }

    return impulse_contact * glm::mat3x3(contact->matContactToWorld);
}
