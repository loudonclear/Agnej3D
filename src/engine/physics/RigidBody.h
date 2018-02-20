#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "engine/components/Component.h"
#include "engine/physics/Collision.h"
#include "engine/components/Transform.h"

class RigidBody : public Component
{
public:
    RigidBody(GameObject *parent, bool isStatic, float mass = 1);

    void init();
    bool isStatic() { return m_isStatic; }
    virtual void onCollide(Collision::ContactData cd);

    void setMass(float mass) { m_mass = mass; m_invMass = 1.f / mass; }
    float getMass() { return m_mass; }

    void integrateVelocities(float dt);
    void integrateForces(float dt);

    glm::vec3 velocity;
    glm::vec3 force;

    bool isGrounded;

protected:

    std::shared_ptr<Transform> m_transform;

private:
    bool m_isStatic;
    float m_mass, m_invMass;
};

#endif // RIGIDBODY_H
