#ifndef RAYCASTRESULT_H
#define RAYCASTRESULT_H


#include <glm/vec3.hpp>

struct RaycastResult
{
public:
    RaycastResult() {}
    RaycastResult(float tvalue, glm::vec3 p, glm::vec3 n) :
    t(tvalue), point(p), normal(n) {}

    float t;
    glm::vec3 point;
    glm::vec3 normal;
};

#endif // RAYCASTRESULT_H
