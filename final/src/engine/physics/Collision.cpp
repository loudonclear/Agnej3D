#include "Collision.h"
#include "engine/physics/ShapeCollider.h"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <numeric>
#include <cmath>
#include <cfloat>
#include <list>

glm::vec3 tripleCrossProduct(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &oldDir) {
    glm::vec3 dir = glm::cross(glm::cross(v1, v2), v1);
//    if (glm::length2(dir) <= FLOAT_EPSILON) {
//        dir = glm::cross(v1, glm::vec3(1, 0, 0));
//    }
//    if (glm::length2(dir) <= FLOAT_EPSILON) {
//        dir = glm::cross(v1, glm::vec3(0, 1, 0));
//    }
//    if (glm::length2(dir) <= FLOAT_EPSILON) {
//        dir = glm::cross(v1, glm::vec3(0, 0, 1));
//    }
    return dir;
}

// hacktank.net && mollyrocket.com/849
bool Collision::gjk(std::shared_ptr<ShapeCollider> s1, std::shared_ptr<ShapeCollider> s2, Simplex &simplex, int *uniqueIdCounter) {

    const unsigned GJK_ITERATION_LIMIT = 20;
    unsigned ITER = 0;

    simplex.clear();
    glm::vec3 dir = s1->getCenterOfMass() - s2->getCenterOfMass();
    if (glm::length2(dir) <= FLOAT_EPSILON) {
        dir = glm::vec3(1, 0, 0);
    }
    SupportPoint s = generateSupport(s1, s2, dir, uniqueIdCounter);

    simplex.add(s);
    dir = -s.v;

    while (true) {
        if (ITER++ >= GJK_ITERATION_LIMIT) return false;
        if (glm::length2(dir) <= FLOAT_EPSILON) {
            return false;
        }
        SupportPoint a = generateSupport(s1, s2, dir, uniqueIdCounter);

        const float proj = glm::dot(a.v, dir);
        if (proj < 0) return false;
        simplex.add(a);

        const glm::vec3 ao = -simplex.a.v;
        bool triangleSimplexTestAboveBelow = true;

        if (simplex.num == 2) {
            const glm::vec3 ab = simplex.b.v - simplex.a.v;
            dir = tripleCrossProduct(ab, ao, dir);

            continue;
        } else if (simplex.num == 3) {
            jmpTriangleSimplex:

            const glm::vec3 ab = simplex.b.v - simplex.a.v;
            const glm::vec3 ac = simplex.c.v - simplex.a.v;
            const glm::vec3 abc = glm::cross(ab, ac);

            if (glm::dot(ao, glm::cross(ab, abc)) > 0) {
                simplex.set(simplex.a, simplex.b);
                dir = tripleCrossProduct(ab, ao, dir);
            } else {

                if (glm::dot(ao, glm::cross(abc, ac)) > 0) {
                    simplex.set(simplex.a, simplex.c);
                    dir = tripleCrossProduct(ac, ao, dir);
                } else {

                    if (glm::dot(ao, abc) > 0) {
                        dir = abc;
                    } else {
                        simplex.set(simplex.a, simplex.c, simplex.b);
                        dir = -abc;
                    }
                }
            }
         continue;
        } else {

            const glm::vec3 ab = simplex.b.v - simplex.a.v;
            const glm::vec3 ac = simplex.c.v - simplex.a.v;

            if (glm::dot(ao, glm::cross(ab, ac)) > 0) {
                simplex.num = 3;
                triangleSimplexTestAboveBelow = false;
                goto jmpTriangleSimplex;
            }

            const glm::vec3 ad = simplex.d.v - simplex.a.v;

            if (glm::dot(ao, glm::cross(ac, ad)) > 0) {
                simplex.set(simplex.a, simplex.c, simplex.d);
                triangleSimplexTestAboveBelow = false;
                goto jmpTriangleSimplex;
            }

            if (glm::dot(ao, glm::cross(ad, ab)) > 0) {
                simplex.set(simplex.a, simplex.d, simplex.b);
                triangleSimplexTestAboveBelow = false;
                goto jmpTriangleSimplex;
            }

            break;
        }
    }

    return true;
}

bool Collision::epa(std::shared_ptr<ShapeCollider> s1, std::shared_ptr<ShapeCollider> s2, Simplex &simplex, int *uniqueIdCounter, ContactBasicData *contactData) {

    const float GROWTH_THRESHOLD = 0.001f;
    const unsigned EPA_ITERATION_LIMIT = 40;
    unsigned ITER = 0;

    std::list<Edge> edges;
    std::list<Triangle> triangles;

    auto addEdge = [&](const SupportPoint &a, const SupportPoint &b) -> void {
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (it->points[0].uniqueId == b.uniqueId && it->points[1].uniqueId == a.uniqueId) {
                edges.erase(it);
                return;
            }
        }
        edges.emplace_back(Edge(a, b));
    };

    triangles.emplace_back(Triangle(simplex.a, simplex.b, simplex.c));
    triangles.emplace_back(Triangle(simplex.a, simplex.c, simplex.d));
    triangles.emplace_back(Triangle(simplex.a, simplex.d, simplex.b));
    triangles.emplace_back(Triangle(simplex.b, simplex.d, simplex.c));

    while (true) {
        if (ITER++ >= EPA_ITERATION_LIMIT) return false;

        std::list<Triangle>::iterator closestTriangleIter;
        float closestTriangleDst = FLT_MAX;
        for (auto it = triangles.begin(); it != triangles.end(); it++) {
            const float dst = glm::dot(it->n, it->points[0].v);

            if (dst < closestTriangleDst) {
                closestTriangleIter = it;
                closestTriangleDst = dst;
            }
        }

        const SupportPoint s = generateSupport(s1, s2, closestTriangleIter->n, uniqueIdCounter);
        const float newDst = glm::dot(closestTriangleIter->n, s.v);

        if (newDst - closestTriangleDst < GROWTH_THRESHOLD) {
            return extrapolateContactInfo(&*closestTriangleIter, contactData);
        }

        for (auto it = triangles.begin(); it != triangles.end(); ) {

            if (glm::dot(it->n, s.v - it->points[0].v) > 0) {
                addEdge(it->points[0], it->points[1]);
                addEdge(it->points[1], it->points[2]);
                addEdge(it->points[2], it->points[0]);
                it = triangles.erase(it);
                continue;
            }
            it++;
        }

        for (auto it = edges.begin(); it != edges.end(); it++) {
            triangles.emplace_back(Triangle(s, it->points[0], it->points[1]));
        }

        edges.clear();
    }

    return false;
}

Collision::SupportPoint Collision::generateSupport(std::shared_ptr<ShapeCollider> s1, std::shared_ptr<ShapeCollider> s2, glm::vec3 &dir, int* uniqueIdCounter) {
    dir = glm::normalize(dir);

    SupportPoint res(uniqueIdCounter ? (*uniqueIdCounter)++ : 0);
    res.supports[0] = s1->getSupport(dir);
    res.supports[1] = s2->getSupport(-dir);
    res.v = res.supports[0] - res.supports[1];

    return res;
}

bool Collision::extrapolateContactInfo(const Triangle *triangle, ContactBasicData *contactData) {

    float originDistance = glm::dot(triangle->n, triangle->points[0].v);

    float u, v, w;
    barycentric(triangle->n * originDistance, triangle->points[0].v, triangle->points[1].v, triangle->points[2].v, &u, &v, &w);

    if (!std::isfinite(u) || !std::isfinite(v) || !std::isfinite(w)) return false;
    //if (fabs(u) > 1.0f || fabs(v) > 1.0f || fabs(w) > 1.0f) return false;

    const glm::vec3 collisionPoint = glm::vec3(u * triangle->points[0].supports[0] + v * triangle->points[1].supports[0] + triangle->points[2].supports[0]);
    const glm::vec3 collisionNormal = -triangle->n;
    const float penetration = originDistance;

    contactData->point = collisionPoint;
    contactData->normal = collisionNormal;
    contactData->penetration = penetration;

    //contactData->triangle = *triangle;

    return true;
}

//bool Collision::extrapolateContactInfo(ContactBasicData *contactData) {
//    return extrapolateContactInfo(contactData->triangle, contactData);
//}

