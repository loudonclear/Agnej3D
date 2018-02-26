#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "System.h"

class ShapeCollider;

class CollisionSystem : public System
{
public:
    CollisionSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void update(float seconds);
    void broadphase();

private:
    std::vector<std::shared_ptr<ShapeCollider>> m_components;
};

#endif // COLLISIONSYSTEM_H
