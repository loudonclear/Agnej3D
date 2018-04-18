#include "PhysicsSystem.h"
#include "engine/physics/RigidBody.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/Collision.h"
#include "engine/physics/Contact.h"

#include <algorithm>
#include <iostream>

const glm::vec3 gravity(0, -1, 0);
const float linearDamping = 0.985f;
const float angularDamping = 0.96f;
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

    //TODO: remove invalid contacts
//    for (Contact &c : contacts) {
//        if (!c.isValid()) {
//            contacts
//        }
//    }

    // Resolve contacts
    resolveContacts();
}

void PhysicsSystem::integrate(float dt) {
    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        std::shared_ptr<RigidBody> rb = *iter;
        std::shared_ptr<ShapeCollider> sc = rb->getShapeCollider();
        std::shared_ptr<Transform> t = sc->getTransform();

        if (rb->invMass != 0 && rb->awake) {
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
                    const float sign = (sc == contact->c1) ? 1.f : -1.f;
                    adjVel += sign * std::max(-sign*glm::dot(adjVel, contact->normal), 0.f) * contact->normal;
                }
            }

            t->translate(adjVel * dt);
            //t->rotate(glm::quat());

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
                if (contact->c1->isStatic() || !contact->c1->getRigidBody()->awake
                 || contact->c2->isStatic() || !contact->c2->getRigidBody()->awake) {
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
            if ((c.c1->isStatic() || !c.c1->getRigidBody()->awake) && (c.c2->isStatic() || !c.c2->getRigidBody()->awake)) {
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
    for (int i = 0; i < m_components.size() - 1; i++) {
        std::shared_ptr<RigidBody> rb1 = m_components.at(i);

        for (int j = 1; j < m_components.size(); j++) {
            std::shared_ptr<RigidBody> rb2 = m_components.at(j);

            if (!(!rb1->isStatic && rb1->awake) && !(!rb2->isStatic && rb2->awake)) {
                continue;
            }

            ContactBasicData c;
            int uniqueId;
            Collision::Simplex s;
            if (Collision::gjk(rb1->getShapeCollider(), rb2->getShapeCollider(), s, &uniqueId) && Collision::epa(rb1->getShapeCollider(), rb2->getShapeCollider(), s, &uniqueId, &c)) {
                newContacts.push_back(c);
            }
        }
    }

    // Add new contacts removing similar ones
    for (Contact &c : newContacts) {
        const std::shared_ptr<ShapeCollider> scCheck = c.c1->contacts.size() < c.c2->contacts.size() ? c.c1 : c.c2;

        for (Contact *oc : scCheck->contacts) {
            if (!oc->isValid()) continue;

            if ((c.c1 == oc->c1 && c.c2 == oc->c2) || (c.c1 == oc->c2 && c.c2 == oc->c1)) {
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

}
