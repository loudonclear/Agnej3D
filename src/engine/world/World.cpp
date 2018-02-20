#include "World.h"
#include <algorithm>
#include <iostream>

World::World()
{

}


void World::addGameObject(const std::string &name, const std::shared_ptr<GameObject> &gameObject) {
    m_gameObjects[name] = gameObject;
    for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter) {
        iter->second->addGameObject(gameObject);
    }
}

void World::addGameObject(const std::string &name, GameObject &&gameObject) {
    std::shared_ptr<GameObject> go = std::make_shared<GameObject>(std::move(gameObject));
    m_gameObjects[name] = go;
    for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter) {
        iter->second->addGameObject(go);
    }
}

std::shared_ptr<GameObject> World::getGameObject(const std::string &name) {
    return m_gameObjects[name];
}

void World::removeGameObject(const std::string &name) {
    for (auto iter = m_systems.begin(); iter != m_systems.end(); ++iter) {
        iter->second->removeGameObject(m_gameObjects[name]);
    }
    m_gameObjects.erase(m_gameObjects.find(name));
}

void World::init() {
    for (auto iter = m_gameObjects.begin(); iter != m_gameObjects.end(); ++iter) {
        iter->second->addToWorld(this);
    }
}
