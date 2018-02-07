#include "GameObject.h"
#include "World.h"

GameObject::GameObject(const std::string &name) : m_world(nullptr), name(name)
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

World* GameObject::getWorld() {
    return m_world;
}
