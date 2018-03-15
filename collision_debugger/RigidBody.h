#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "glm/glm.hpp"
#include <memory>
#include "Transform.h"

class RigidBody
{
public:
    RigidBody(std::shared_ptr<Transform> &t, float mass = 1);

    void setMass(float mass) { m_mass = mass; m_invMass = 1.f / mass; }
    float getMass() { return m_mass; }

    //void integrateVelocities(float dt);
    //void integrateForces(float dt);

    std::shared_ptr<Transform> getTransformFuture(float seconds);

    glm::vec3 velocity;
    glm::vec3 force;

protected:

    std::shared_ptr<Transform> m_transform;

private:
    float m_mass, m_invMass;
};
#endif // RIGIDBODY_H
