#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H


#include "System.h"

class RigidBody;
class ShapeCollider;
class Contact;

class PhysicsSystem : public System
{
public:
    PhysicsSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void update(float dt);
    void integrate(float dt);
    void updateSleepWake(std::shared_ptr<RigidBody> &rb, std::shared_ptr<ShapeCollider> &sc, const glm::vec3 &dp, const glm::vec3 &dr, float dt);

    void generateContacts();
    void resolveContacts();

private:
    std::vector<std::shared_ptr<RigidBody>> m_components;
    std::vector<Contact> contacts;
};

#endif // PHYSICSSYSTEM_H
