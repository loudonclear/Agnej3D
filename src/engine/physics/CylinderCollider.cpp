#include "CylinderCollider.h"


CylinderCollider::CylinderCollider(GameObject *parent, Transform colliderTransform) : Component(parent), ShapeCollider(parent, colliderTransform)
{
}

bool CylinderCollider::pointInside(const glm::vec3 &point) {
    glm::vec3 localPoint = m_transform->inverseTransformPoint(point);
    return localPoint.y > -0.5f && localPoint.y < 0.5f && localPoint.x*localPoint.x + localPoint.z * localPoint.z < 0.25f ? true : false;
}

glm::vec3 CylinderCollider::getSupport(const glm::vec3 &dir) {
    glm::vec3 localDir = glm::normalize(m_transform->inverseRotateVector(dir));

    glm::vec3 support;
    support.y = localDir.y < 0.f ? -0.5f : 0.5f;

    float s = sqrt(localDir.x * localDir.x + localDir.z * localDir.z);
    if (s > FLOAT_EPSILON) {
        support.x = localDir.x * 0.5f / s;    
        support.z = localDir.z * 0.5f / s;
    } else {
        support.x = 0.5f;
        support.z = 0.f;
    }

    return m_transform->transformPoint(m_colliderTransform.transformPoint(support));
}


