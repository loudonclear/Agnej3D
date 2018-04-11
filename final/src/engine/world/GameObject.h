#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "engine/components/Component.h"
#include "engine/components/Transform.h"
#include "engine/components/TypeMap.h"

#include <memory>
#include <vector>

class World;

class GameObject
{
public:
    GameObject(const std::string &name);
    const std::string name;

    void addToWorld(World *world);
    void removeFromWorld(World *world);
    World* getWorld();

    template <typename ComponentType>
    std::shared_ptr<ComponentType> getComponent() {
        auto it = m_components.find<ComponentType>();
        if (it == m_components.end()) return nullptr;
        return std::static_pointer_cast<ComponentType>(it->second);
    }

    template <typename ComponentType>
    void addComponent(std::shared_ptr<ComponentType> &c) {
        m_components.put<ComponentType>(c);
    }

    template <typename ComponentType>
    void removeComponent() {
        m_components.remove<ComponentType>();
    }

protected:
    World *m_world;

private:

    TypeMap<std::shared_ptr<Component>> m_components;
};

#endif // GAMEOBJECT_H
