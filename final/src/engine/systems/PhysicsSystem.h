#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "System.h"

class ShapeCollider;

class PhysicsSystem : public System
{
public:
    PhysicsSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void update(float seconds);
    void broadphase();

private:
    std::vector<std::shared_ptr<ShapeCollider>> m_components;
};

#endif // PHYSICSSYSTEM_H
