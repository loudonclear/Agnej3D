#ifndef SHAPECOLLIDER_H
#define SHAPECOLLIDER_H

#include "engine/physics/Contact.h"
#include "engine/components/Component.h"
#include "engine/components/Transform.h"
#include "engine/physics/RigidBody.h"
#include "engine/world/GameObject.h"
#include "engine/physics/Collision.h"
#include "RaycastResult.h"
#include "Ray.h"

#include <cmath>

const float FLOAT_EPSILON = 1e-4f;
#define SIGN(n) ((n)<0 ? -1:1)

class ShapeCollider : public virtual Component
{
public:
    friend class RigidBody;

    virtual void init();

    std::shared_ptr<Transform> getTransform();
    std::shared_ptr<RigidBody> getRigidBody();

    bool isStatic();

    //virtual float volume() = 0;
    virtual glm::vec3 getSupport(const glm::vec3 &dir) = 0;
    virtual glm::vec3 getCenterOfMass();
    //virtual bool pointInside(const glm::vec3 &point) = 0;
    //virtual bool raycast(const Ray &ray) = 0;

    std::vector<Contact *> contacts;

protected:
    ShapeCollider(GameObject *parent, Transform colliderTransform);

    std::shared_ptr<Transform> m_transform;
    std::shared_ptr<RigidBody> m_rigidbody;

    Transform m_colliderTransform;

    bool SolveQuadratic(float a, float b, float c, float &t) {
        float discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0) return false;
        t = (-b - sqrt(discriminant)) / (2.0 * a);
        return (t > -FLOAT_EPSILON);
    }

private:

    glm::vec3 com;

};

#endif // SHAPECOLLIDER_H
