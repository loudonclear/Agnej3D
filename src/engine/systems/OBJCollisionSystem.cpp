#include "OBJCollisionSystem.h"

#include "engine/physics/ShapeCollider.h"
#include "engine/physics/SphereCollider.h"
#include "engine/physics/TriangleCollider.h"
#include "engine/graphics/Obj.h"
#include "engine/physics/Collision.h"

#include <algorithm>
#include <iostream>

OBJCollisionSystem::OBJCollisionSystem(std::shared_ptr<OBJ> o) : obj(o)
{
}

void OBJCollisionSystem::collide(std::shared_ptr<ShapeCollider> ellipsoid, float time) {

    glm::vec3 start = ellipsoid->getCenter();
    glm::vec3 end = ellipsoid->getCenter() + time * ellipsoid->getRigidBody()->velocity;

    if (obj == nullptr) {
        ellipsoid->getTransform()->setPosition(end);
        return;
    }

    for (int i = 0; i < 6; i++) {
        RaycastResult lambda;
        lambda.t = FLT_MAX;

        for (OBJ::Triangle *tri : obj->triangles) {
            std::shared_ptr<TriangleCollider> trishape = std::make_shared<TriangleCollider>(tri->vertices[0], tri->vertices[1], tri->vertices[2]);

            RaycastResult test = continuousConvexCollision(ellipsoid, trishape, start, end);
            if (test.t >= 0.f && test.t < lambda.t) {
                lambda = test;
            }
        }
        if (lambda.t >= 0.f && lambda.t <= 1.f) {
            start = start + lambda.t*(end - start) + 0.0001f * lambda.normal;
            glm::vec3 v = (end - start);

            float ndotup = glm::dot(lambda.normal, glm::vec3(0, 1, 0));
            glm::vec3 dir;
            if (ndotup > 0.0001f) {
                dir = v - glm::dot(lambda.normal, v) / ndotup * glm::vec3(0, 1, 0);
                ellipsoid->getRigidBody()->isGrounded = true;
                ellipsoid->getRigidBody()->velocity.y = 0;
            } else {
                dir = v - glm::dot(lambda.normal, v) * lambda.normal;
            }

            end = start + dir;
        } else {
            break;
        }
    }

    ellipsoid->getTransform()->setPosition(end);
}

bool SolveQuadratic(float a, float b, float c, float &t) {
    float discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0) return false;
    t = (-b - sqrt(discriminant)) / (2.0 * a);
    return (t > -FLOAT_EPSILON);
}

bool OBJCollisionSystem::edgeTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C, const glm::vec3 &D, const glm::vec3 &P, float &t, glm::vec3 &pp) {
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

bool OBJCollisionSystem::vertexTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &V, const glm::vec3 &P, float &t) {
    const glm::vec3 AB = B - A;
    const glm::vec3 AV = V - A;

    const float a = glm::dot(AB, AB);
    const float b = -2.f*glm::dot(AB, AV);
    const float c = glm::dot(AV, AV) - 1;
    return SolveQuadratic(a, b, c, t);
}

RaycastResult OBJCollisionSystem::continuousConvexCollision(std::shared_ptr<ShapeCollider> &s1, std::shared_ptr<TriangleCollider> &s2, glm::vec3 a, glm::vec3 b) {
    RaycastResult res;
    res.t = -1;

    const glm::vec3 scale = s1->getTransform()->getScale();

    TriangleCollider scaled = TriangleCollider(s2->getV0() / scale, s2->getV1() / scale, s2->getV2() / scale);
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
