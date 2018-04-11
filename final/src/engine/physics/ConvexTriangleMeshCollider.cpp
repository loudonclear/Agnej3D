#include "ConvexTriangleMeshCollider.h"

ConvexTriangleMeshCollider::ConvexTriangleMeshCollider(std::shared_ptr<GameObject> &parent, std::vector<glm::vec3> vertices) : ShapeCollider(parent), vertices(vertices)
{
}

glm::vec3 ConvexTriangleMeshCollider::getSupport(const glm::vec3 &dir) {
    const glm::vec3 localDir = glm::normalize(m_transform->inverseRotateVector(dir));

    glm::vec3 support;
    float best;
    for (const glm::vec3 &v : vertices) {
        float t = glm::dot(localDir, v);
        if (t > best) {
            best = t;
            support = v;
        }
    }

    return m_transform->transformPoint(support);
}
