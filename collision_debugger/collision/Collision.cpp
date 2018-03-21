#include "Collision.h"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

bool Collision::edgeTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C, const glm::vec3 &D, const glm::vec3 &P, float &t, glm::vec3 &pp) {
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

        pp = C + glm::dot(CP, CD) / CDdotCD * CD;

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

RaycastResult Collision::continuousConvexCollision(std::shared_ptr<SphereShape> &s1, std::shared_ptr<Transform> &s1t, std::shared_ptr<TriangleShape> &s2, glm::vec3 a, glm::vec3 b) {
    RaycastResult res;
    res.t = -1;

    const glm::vec3 scale = s1t->getScale();

    TriangleShape scaled = TriangleShape(s2->getV0() / scale, s2->getV1() / scale, s2->getV2() / scale);
    const glm::vec3 n = scaled.getNormal();

    a /= scale;
    b /= scale;

    glm::vec3 ab = b - a;
    const float ndotab = glm::dot(n, ab);
    if (fabs(ndotab) < FLOAT_EPSILON) return res;

    float t = -glm::dot(n, (a - n) - s2->getV0() / scale) / ndotab;
    if (t < 0.f) return res;
    glm::vec3 point = a - n + t * (ab);


    if (scaled.pointWithinEdges(point)) {
        res.t = t;
        res.point = point;
    } else {
        glm::vec3 pp;
        res.t = FLT_MAX;
        if (edgeTest(a, b, scaled.getV0(), scaled.getV1(), point, t, pp) && t < res.t) {
            res.t = t;
            res.point = pp;
        }
        if (edgeTest(a, b, scaled.getV1(), scaled.getV2(), point, t, pp) && t < res.t) {
            res.t = t;
            res.point = pp;
        }
        if (edgeTest(a, b, scaled.getV2(), scaled.getV0(), point, t, pp) && t < res.t) {
            res.t = t;
            res.point = pp;
        }

        if (vertexTest(a, b, scaled.getV0(), point, t) && t < res.t) {
            res.t = t;
            res.point = scaled.getV0();
        }
        if (vertexTest(a, b, scaled.getV1(), point, t) && t < res.t) {
            res.t = t;
            res.point = scaled.getV1();
        }
        if (vertexTest(a, b, scaled.getV2(), point, t) && t < res.t) {
            res.t = t;
            res.point = scaled.getV2();
        }
    }

    res.normal = glm::normalize((a + t * (ab) - res.point) / scale);
    res.point = res.point * scale;

    return res;
}
