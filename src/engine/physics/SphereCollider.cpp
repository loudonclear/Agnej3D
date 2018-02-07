#include "SphereCollider.h"

SphereCollider::SphereCollider(std::shared_ptr<GameObject> &parent) : ShapeCollider(parent)
{
}

//bool SphereCollider::pointInside(const glm::vec3 &point) {
//    glm::vec3 res = m_transform.transformPoint(point);
//    return glm::length2(res) < m_radiusSquared ? true : false;
//}

//bool SphereCollider::raycast(const Ray &ray, RaycastResult &result) {
//    Ray localRay = m_transform.transformRay(ray);

//    float t;
//    float a = glm::length2(localRay.direction);
//    float b = 2.0 * glm::dot(localRay.direction, localRay.start);
//    float c = glm::length2(localRay.start) - m_radiusSquared;

//    if (SolveQuadratic(a, b, c, t)) {
//        glm::vec3 worldHit = ray.start + t*ray.direction;
//        result = RaycastResult(this, worldHit, m_transform.transformNormal(worldHit));
//        return true;
//    }
//    return false;
//}

glm::vec3 SphereCollider::getSupport(const glm::vec3 &dir) {
    glm::vec3 localDir = glm::normalize(m_transform->inverseRotateVector(dir));

    glm::vec3 support(0.5f * localDir);
    return m_transform->transformPoint(support);
}
