#include "CylinderCollider.h"

#include <iostream>

CylinderCollider::CylinderCollider(std::shared_ptr<GameObject> &parent) : ShapeCollider(parent)
{
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

    return m_transform->transformPoint(support);
}

