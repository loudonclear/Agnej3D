#ifndef RAYCASTRESULT_H
#define RAYCASTRESULT_H

#include <glm/vec3.hpp>

class ShapeCollider;

struct RaycastResult
{
public:
    RaycastResult(ShapeCollider *collider, glm::vec3 worldPoint, glm::vec3 worldNormal) :
    collider(collider), worldPoint(worldPoint), worldNormal(worldNormal) {}

    ShapeCollider *collider;
    glm::vec3 worldPoint;
    glm::vec3 worldNormal;
};

#endif // RAYCASTRESULT_H
