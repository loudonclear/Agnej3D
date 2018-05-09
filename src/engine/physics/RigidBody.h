#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "components/Component.h"
#include <glm/glm.hpp>

class ShapeCollider;
class Transform;

class RigidBody : public virtual Component {

public:
    RigidBody(GameObject* parent, float density = 1.f);
    virtual ~RigidBody() {}

    void init();

    void addForce(const glm::vec3 &f);
    void addForceWorld(const glm::vec3 &f, const glm::vec3 &p);
    void addTorque(const glm::vec3 &t);


    std::shared_ptr<ShapeCollider> getCollider() { return collider; }
    std::shared_ptr<Transform> getTransform() { return transform; }


	float density;
    bool awake;
    float motion;
    bool isStatic;
    bool canSleep;
    bool interaction;

    glm::vec3 velocity;
    glm::vec3 angularVelocity;

    glm::vec3 acceleration;
    glm::vec3 force;
    glm::vec3 torque;

protected:

    std::shared_ptr<Transform> transform;
    std::shared_ptr<ShapeCollider> collider;

};

#endif // RIGIDBODY_H
