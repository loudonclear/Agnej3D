#include "BoxCollider.h"

#include "physics/RigidBody.h"
#include "components/Transform.h"

BoxCollider::BoxCollider(GameObject* parent, float mass) : Component(parent), ShapeCollider(parent) {

    if (mass <= 0.f) {
        this->mass = 0;
        inertialTensor = glm::mat4x4(0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,0,
                                     0,0,0,1);

        inverseMass = 0;
        invInertialTensor = glm::mat4x4(1.f);
    } else {
        this->mass = mass;
        inverseMass = 1.f;

        const float s = 1.f/12.f * mass;

        inertialTensor = glm::mat4x4(1.f);
        inertialTensor[0][0] = s * (2);
        inertialTensor[1][1] = s * (2);
        inertialTensor[2][2] = s * (2);

        invInertialTensor = glm::inverse(inertialTensor);
    }
}

void BoxCollider::init() {
    ShapeCollider::init();
    extent = glm::length(0.5f * transform->getScale());
}

glm::vec3 BoxCollider::getSupportPointLocal(const glm::vec3 &dirWorldSpace) const {
    const glm::vec3 localDirection = _worldToLocal(dirWorldSpace);

    const glm::vec3 extentInDirection(
        SIGN(localDirection.x)*0.5f,
        SIGN(localDirection.y)*0.5f,
        SIGN(localDirection.z)*0.5f);

    return extentInDirection;
}

glm::vec3 BoxCollider::getCenterOfMass() const {
    return transform->getPosition();
}

bool BoxCollider::raycast(const Ray &ray, RaycastResult &res) const {
    const glm::vec3 start = transform->inverseTransformPoint(ray.start);
    const glm::vec3 dir = transform->inverseRotateVector(ray.direction);

    const glm::vec3 minT = (glm::vec3(-0.5f, -0.5f, -0.5f) - start) / dir;
    const glm::vec3 maxT = (glm::vec3(0.5f, 0.5f, 0.5f) - start) / dir;

    const glm::vec3 t1 = Min(minT, maxT);
    const glm::vec3 t2 = Max(minT, maxT);

    const float tNear = Max(t1);
    const float tFar = Min(t2);

    res.t = tNear;
    res.point = ray.start + ray.direction * tNear;

    return (tNear < tFar && tFar > FLOAT_EPSILON);
}
