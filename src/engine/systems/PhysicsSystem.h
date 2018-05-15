#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H


#include "System.h"
#include "pool_vector.h"
#include <list>
#include "physics/RaycastResult.h"

class RigidBody;
class ShapeCollider;
class Contact;

class PhysicsSystem : public System
{
public:
    PhysicsSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    bool raycast(const Ray &ray, RaycastResult &res, std::shared_ptr<RigidBody> &hit);
    void update(float dt);

    std::vector<std::shared_ptr<RigidBody>> getComponents() { return m_components; }

private:
    std::vector<std::shared_ptr<RigidBody>> m_components;
    pool_vector<Contact> contacts;
};

#endif // PHYSICSSYSTEM_H
