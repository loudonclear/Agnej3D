#include "PhysicsSystem.h"
#include "physics/RigidBody.h"
#include "physics/ShapeCollider.h"
#include "engine/physics/Contact.h"

#include "physics/EulerIntegrator.h"
#include "physics/ContactGenerator.h"
#include "physics/ImpulseContactResolver.h"

#include <glm/gtx/string_cast.hpp>

#include <algorithm>
#include <iostream>


PhysicsSystem::PhysicsSystem()
{
    contacts.reserve(1000);
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

bool PhysicsSystem::raycast(const Ray &ray, RaycastResult &res, std::shared_ptr<RigidBody> &hit) {

    res.t = FLT_MAX;
    for (std::shared_ptr<RigidBody> rb : m_components) {
        RaycastResult temp;
        if (rb->getCollider()->raycast(ray, temp) && temp.t >= 0.f && temp.t < res.t) {
            res = temp;
            hit = rb;
        }
    }
    return res.t < FLT_MAX;
}

void PhysicsSystem::update(float dt) {
    // Collision testing from Hacktank tutorial

    // Integration
    EulerIntegrator::integrateBodies(m_components, dt);

    if (m_components.size() < 1) return;

    // Contact generation
    ContactGenerator::frame(m_components, contacts);

    // Remove invalid contacts
    for (auto it = contacts.begin(); it != contacts.end(); it++) {
        if (!it->isValid()) {
            contacts.remove(it);
        }
    }

    // Resolve contacts
    ImpulseContactResolver::frame(contacts);
}
