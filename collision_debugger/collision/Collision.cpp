#include "Collision.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

bool Collision::edgeTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C, const glm::vec3 &D, const glm::vec3 &P, float &t) {
    const glm::vec3 AB = B - A;
    const glm::vec3 CD = D - C;
    const glm::vec3 CA = A - C;
    const glm::vec3 CP = P - C;

    const glm::vec3 ABcrossCD = glm::cross(AB, CD);
    const glm::vec3 CAcrossCD = glm::cross(CA, CD);

    const float CPdotCD = glm::dot(CP, CD);
    const float CDdotCD = glm::dot(CD, CD);

    if (0.f < CPdotCD && CPdotCD < CDdotCD) {
        const float a = glm::dot(ABcrossCD, ABcrossCD);
        const float b = 2.f*glm::dot(ABcrossCD, CAcrossCD);
        const float c = glm::dot(CAcrossCD, CAcrossCD) - CDdotCD;
        return SolveQuadratic(a, b, c, t);
    }
    return false;
}

bool Collision::vertexTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &V, const glm::vec3 &P, float &t) {
    const glm::vec3 AB = B - A;
    const glm::vec3 AV = V - A;

    const float a = glm::dot(AB, AB);
    const float b = -2.f*glm::dot(AB, AV);
    const float c = glm::dot(AV, AV) - 1;
    return SolveQuadratic(a, b, c, t);
}

float Collision::continuousConvexCollision(std::shared_ptr<SphereShape> &s1, std::shared_ptr<Transform> &s1t, std::shared_ptr<TriangleShape> &s2, glm::vec3 a, glm::vec3 b) {
    const glm::vec3 n = s2->getNormal();
    glm::vec3 ab = b - a;
    const float ndotab = glm::dot(n, ab);
    if (fabs(ndotab) < FLOAT_EPSILON) return -1;

    float t = -glm::dot(n, (a - n) - s2->getV0()) / ndotab;
    if (t < 0.f) return -1;
    glm::vec3 point = a + t * (ab);

    std::shared_ptr<TriangleShape> scaled = std::make_shared<TriangleShape>(s2->getV0() / s1t->getScale(), s2->getV1() / s1t->getScale(), s2->getV2() / s1t->getScale());

    if (s2->pointWithinEdges(point)) {
        return t;
    } else {
        t = FLT_MAX;
        float test;
        if (edgeTest(a, b, scaled->getV0(), scaled->getV1(), point, test) && test < t) t = test;
        if (edgeTest(a, b, scaled->getV1(), scaled->getV2(), point, test) && test < t) t = test;
        if (edgeTest(a, b, scaled->getV2(), scaled->getV0(), point, test) && test < t) t = test;

        if (vertexTest(a, b, scaled->getV0(), point, test) && test < t) t = test;
        if (vertexTest(a, b, scaled->getV1(), point, test) && test < t) t = test;
        if (vertexTest(a, b, scaled->getV2(), point, test) && test < t) t = test;
    }

    if (t == FLT_MAX) {
        return -1;
    } else {
        return t;
    }
}
