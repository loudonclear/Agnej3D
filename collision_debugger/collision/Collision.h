#ifndef COLLISION_H
#define COLLISION_H

#include <memory>
#include "CollisionShape.h"
#include "ConvexShape.h"
#include "TriangleShape.h"
#include "SphereShape.h"

class Collision
{
public:

    static float continuousConvexCollision(std::shared_ptr<SphereShape> &s1, std::shared_ptr<Transform> &s1t, std::shared_ptr<TriangleShape> &s2, glm::vec3 a, glm::vec3 b);

    static bool SolveQuadratic(float a, float b, float c, float &t) {
        float discriminant = b * b - 4.f * a * c;
        if (discriminant < 0.f) return false;
        t = (-b - sqrt(discriminant)) / (2.f * a);
        float t2 = (-b + sqrt(discriminant)) / (2.f * a);
        if (t > 0 && t2 < t) t = t2;
        else if (t < 0) t = t2;

        return (t > -FLOAT_EPSILON);
    }

private:

    static bool edgeTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C, const glm::vec3 &D, const glm::vec3 &P, float &t);
    static bool vertexTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &V, const glm::vec3 &P, float &t);
};

#endif // COLLISION_H
