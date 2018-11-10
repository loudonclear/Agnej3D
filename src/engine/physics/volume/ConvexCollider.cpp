#include "ConvexCollider.h"

#include "physics/RigidBody.h"
#include "components/Transform.h"
#include <iostream>

ConvexCollider::ConvexCollider(GameObject* parent, std::shared_ptr<ConvexHull> &hull, float mass) : Component(parent), ShapeCollider(parent), hull(hull) {

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

        const glm::vec3 size = hull->max - hull->min;
        const glm::vec3 sizesq = glm::vec3(size.x * size.x, size.y * size.y, size.z * size.z);
        const float s = 1.f / 12.f * mass;

        inertialTensor = glm::mat4x4(1.f);
        inertialTensor[0][0] = s * (sizesq.y + sizesq.z);
        inertialTensor[1][1] = s * (sizesq.x + sizesq.z);
        inertialTensor[2][2] = s * (sizesq.x + sizesq.y);

        invInertialTensor = glm::inverse(inertialTensor);
    }
}

void ConvexCollider::init() {
    ShapeCollider::init();
    extent = glm::length(hull->max - hull->min);
}

glm::vec3 ConvexCollider::getSupportPointLocal(const glm::vec3 &dirWorldSpace) const {
    const glm::vec3 localDirection = _worldToLocal(dirWorldSpace);

    float best = FLT_MIN;
    glm::vec3 res = glm::vec3(0.f);
    for (const glm::vec3 &v : hull->vertices) {
        float temp = glm::dot(v, localDirection);
        if (best < temp) {
            best = temp;
            res = v;
        }
    }

    return res;
}

glm::vec3 ConvexCollider::getCenterOfMass() const {
    return transform->transformPoint(hull->com);
}

bool ConvexCollider::raycast(const Ray &ray, RaycastResult &res) const {

    bool found = false;
    RaycastResult temp;
    res.t = FLT_MAX;

    for (const glm::ivec3 &face : hull->faces) {
        const glm::vec3 v0 = transform->transformPoint(hull->vertices.at(face[0]));
        const glm::vec3 v1 = transform->transformPoint(hull->vertices.at(face[1]));
        const glm::vec3 v2 = transform->transformPoint(hull->vertices.at(face[2]));
        const glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        const float ndotd = glm::dot(normal, ray.direction);
        if (fabs(ndotd) < FLOAT_EPSILON) continue;

        float tvalue = -glm::dot(normal, ray.start - v0) / ndotd;
        temp.t = tvalue;
        temp.point = ray.start + tvalue*ray.direction;
        temp.normal = normal;

        if (glm::dot(normal, glm::cross(v1 - v0, temp.point - v0)) < FLOAT_EPSILON) continue;
        if (glm::dot(normal, glm::cross(v2 - v1, temp.point - v1)) < FLOAT_EPSILON) continue;
        if (glm::dot(normal, glm::cross(v0 - v2, temp.point - v2)) < FLOAT_EPSILON) continue;

        if (temp.t < res.t) res = temp;
        found = true;
    }

    return found;
}
