#include "SphereCollider.h"

SphereCollider::SphereCollider(GameObject *parent, Transform colliderTransform) : Component(parent), ShapeCollider(parent, colliderTransform)
{
}

bool SphereCollider::pointInside(const glm::vec3 &point) {
    glm::vec3 localPoint = m_transform->inverseTransformPoint(point);
    return glm::length2(localPoint) <= 0.25f ? true : false;
}

bool SphereCollider::raycast(const Ray &ray, std::shared_ptr<Transform> t, RaycastResult &result) {
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

glm::vec3 SphereCollider::getSupport(const glm::vec3 &dir) {
    glm::vec3 localDir = glm::normalize(m_transform->inverseRotateVector(dir));

    glm::vec3 support(0.5f * localDir);
    return m_transform->transformPoint(m_colliderTransform.transformPoint(support));
}
