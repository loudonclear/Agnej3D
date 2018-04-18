#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "engine/components/Component.h"
#include "engine/physics/Collision.h"
#include "engine/components/Transform.h"

class ShapeCollider;

class RigidBody : public virtual Component
{
public:
    RigidBody(GameObject *parent, bool isStatic = false, float mass = 1);

    void init();

    void setMass(float mass) { mass = mass; invMass = 1.f / mass; }
    float getMass() { return mass; }

    void addForceAtPoint(const glm::vec3 &f, const glm::vec3 &p);

    std::shared_ptr<ShapeCollider> getShapeCollider() { return m_collider; }

    bool isStatic;
    bool canSleep, awake;
    float mass, invMass;
    float motion;
    glm::vec3 velocity;
    glm::vec3 angularVelocity;
    glm::vec3 force;
    glm::vec3 torque;
    glm::vec3 lastAcceleration;
    glm::mat4x4 invInertialTensor;



protected:

    std::shared_ptr<Transform> m_transform;
    std::shared_ptr<ShapeCollider> m_collider;
};

#endif // RIGIDBODY_H
