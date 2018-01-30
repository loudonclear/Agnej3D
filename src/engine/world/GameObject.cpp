#include "GameObject.h"
#include "World.h"

GameObject::GameObject() : m_world(nullptr)
{

}

void GameObject::addToWorld(World *world) {
    m_world = world;

    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        iter->second->init();
    }

}

void GameObject::removeFromWorld(World *world) {
    assert(m_world = world);
    m_world = nullptr;


}
