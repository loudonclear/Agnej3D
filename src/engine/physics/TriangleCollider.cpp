#include "TriangleCollider.h"

TriangleCollider::TriangleCollider(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) {
    vertices[0] = v0;
    vertices[1] = v1;
    vertices[2] = v2;

    normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
    plane = glm::vec4(normal, -glm::dot(normal, v0));
}

glm::vec3 TriangleCollider::getSupport(const glm::vec3 &dir, std::shared_ptr<Transform> t) {
    unsigned int maxInd;
    float max = std::numeric_limits<float>::min();
    for (unsigned int i = 0; i < 3; i++) {
        const float dot = glm::dot(dir, vertices[i]);
        if (dot > max) {
            max = dot;
            maxInd = i;
        }
    }

    glm::vec3 support = vertices[maxInd];
    return support;
}

bool TriangleCollider::pointWithinEdges(const glm::vec3 &point) {
    if (glm::dot(normal, glm::cross(vertices[1] - vertices[0], point - vertices[0])) < FLOAT_EPSILON) return false;
    if (glm::dot(normal, glm::cross(vertices[2] - vertices[1], point - vertices[1])) < FLOAT_EPSILON) return false;
    if (glm::dot(normal, glm::cross(vertices[0] - vertices[2], point - vertices[2])) < FLOAT_EPSILON) return false;

    return true;
}

bool TriangleCollider::pointInside(const glm::vec3 &point, std::shared_ptr<Transform> t) {
    if (!glm::dot(glm::vec4(point, 1), plane) < FLOAT_EPSILON) return false;

    return pointWithinEdges(point);
}

bool TriangleCollider::raycast(const Ray &ray, std::shared_ptr<Transform> t, RaycastResult &result) {

    const float ndotd = glm::dot(normal, ray.direction);
    if (fabs(ndotd) < 0.f) return false;

    float tvalue = -glm::dot(normal, ray.start - vertices[0]) / ndotd;
    result.t = tvalue;
    result.point = ray.start + tvalue*ray.direction;
    result.normal = normal;

    return pointWithinEdges(result.point);
}
