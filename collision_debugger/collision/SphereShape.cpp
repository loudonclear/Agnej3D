#include "SphereShape.h"

SphereShape::SphereShape() {}

glm::vec3 SphereShape::getSupport(const glm::vec3 &dir, std::shared_ptr<Transform> t) {
    glm::vec3 localDir = glm::normalize(t->inverseRotateVector(dir));

    glm::vec3 support(0.5f * localDir);
    return t->transformPoint(support);
}

bool SphereShape::pointInside(const glm::vec3 &point, std::shared_ptr<Transform> t) {
    glm::vec3 localPoint = t->inverseTransformPoint(point);
    return glm::length2(localPoint) <= 0.25f ? true : false;
}

bool SphereShape::raycast(const Ray &ray, std::shared_ptr<Transform> t, RaycastResult &result) {
    Ray localRay = t->transformRay(ray);

    float tvalue;
    float a = glm::length2(localRay.direction);
    float b = 2.0 * glm::dot(localRay.direction, localRay.start);
    float c = glm::length2(localRay.start) - 0.25f;

    if (SolveQuadratic(a, b, c, tvalue)) {
        glm::vec3 worldHit = ray.start + tvalue*ray.direction;
        result = RaycastResult(tvalue, worldHit, t->transformNormal(worldHit));
        return true;
    }
    return false;
}
