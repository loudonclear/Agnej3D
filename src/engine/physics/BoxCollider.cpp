#include "BoxCollider.h"

BoxCollider::BoxCollider(GameObject *parent, Transform colliderTransform) : ShapeCollider(parent, colliderTransform)
{
}

//bool BoxCollider::pointInside(const glm::vec3 &point) {
//    glm::vec3 localPoint = m_transform.transformPoint(point);
//    return localPoint.x > -m_halfSize.x && localPoint.x < m_halfSize.x &&
//           localPoint.y > -m_halfSize.y && localPoint.y < m_halfSize.y &&
//           localPoint.z > -m_halfSize.z && localPoint.z < m_halfSize.z;
//}

//bool BoxCollider::raycast(const Ray &ray, RaycastResult &result) {

//}

glm::vec3 BoxCollider::getSupport(const glm::vec3 &dir) {
    glm::vec3 localDir = glm::normalize(m_transform->inverseRotateVector(dir));

    glm::vec3 support(SIGN(localDir.x) * 0.5f, SIGN(localDir.y) * 0.5f, SIGN(localDir.z) * 0.5f);

    return m_transform->transformPoint(m_colliderTransform.transformPoint(support));
}
