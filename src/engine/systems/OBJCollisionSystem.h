#ifndef OBJCOLLISIONSYSTEM_H
#define OBJCOLLISIONSYSTEM_H

#include <memory>
#include "glm/glm.hpp"
#include "engine/physics/RaycastResult.h"

class OBJ;
class ShapeCollider;
class TriangleCollider;

class OBJCollisionSystem
{
public:
    OBJCollisionSystem(std::shared_ptr<OBJ> o);

    void collide(std::shared_ptr<ShapeCollider> ellipsoid, float time);
    bool edgeTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &C, const glm::vec3 &D, const glm::vec3 &P, float &t, glm::vec3 &pp);
    bool vertexTest(const glm::vec3 &A, const glm::vec3 &B, const glm::vec3 &V, const glm::vec3 &P, float &t);
    RaycastResult continuousConvexCollision(std::shared_ptr<ShapeCollider> &s1, std::shared_ptr<TriangleCollider> &s2, glm::vec3 a, glm::vec3 b);

private:
    std::shared_ptr<OBJ> obj;
};

#endif // OBJCOLLISIONSYSTEM_H
