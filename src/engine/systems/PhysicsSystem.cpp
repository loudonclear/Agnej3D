#include "PhysicsSystem.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/Collision.h"

#include <algorithm>
#include <iostream>

PhysicsSystem::PhysicsSystem()
{

}

void PhysicsSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<ShapeCollider> component = go->getComponent<ShapeCollider>();
    if (component != nullptr) m_components.push_back(component);
}

void PhysicsSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<ShapeCollider> component = go->getComponent<ShapeCollider>();
    if (component != nullptr)
        m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void PhysicsSystem::update(float seconds) {

    broadphase();

}

void PhysicsSystem::broadphase() {
    if (m_components.size() < 2) return;

    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->get()->setColliding(false);
    }

    for (auto iter1 = m_components.begin(); iter1 != --m_components.end(); ++iter1) {
        for (auto iter2 = ++m_components.begin(); iter2 != m_components.end(); ++iter2) {
            Collision::ContactData cd;
            ShapeCollider* s1 = iter1->get();
            ShapeCollider* s2 = iter2->get();
            cd.s1 = s1;
            cd.s2 = s2;

            //TODO uncomment
            //if(!(s1->isStatic() && s2->isStatic())) continue;
            if (Collision::collide(cd)) {
                s1->setColliding(true);
                s2->setColliding(true);

                s1->onCollide(cd);

                cd.s2 = s1;
                cd.s1 = s2;
                cd.contactNormal = -cd.contactNormal;
                s2->onCollide(cd);
            }
        }
    }
}

//void PhysicsSystem::narrowphase() {

//}
