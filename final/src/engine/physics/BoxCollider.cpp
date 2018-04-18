#include "BoxCollider.h"

#include <iostream>
#include "glm/gtx/string_cast.hpp"

BoxCollider::BoxCollider(GameObject *parent) : Component(parent), ShapeCollider(parent)
{
}

bool BoxCollider::pointInside(const glm::vec3 &point) {
    glm::vec3 localPoint = m_transform->inverseTransformPoint(point);
    return localPoint.x > -0.5f && localPoint.x < 0.5f &&
           localPoint.y > -0.5f && localPoint.y < 0.5f &&
           localPoint.z > -0.5f && localPoint.z < 0.5f;
}

//bool BoxCollider::raycast(const Ray &ray, RaycastResult &result) {

//}

glm::vec3 BoxCollider::getSupport(const glm::vec3 &dir) {
    glm::vec3 localDir = glm::normalize(m_transform->inverseRotateVector(dir));

    glm::vec3 support(SIGN(localDir.x) * 0.5f, SIGN(localDir.y) * 0.5f, SIGN(localDir.z) * 0.5f);

    return m_transform->transformPoint(support);
}
