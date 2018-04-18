#ifndef CONVEXTRIANGLEMESHCOLLIDER_H
#define CONVEXTRIANGLEMESHCOLLIDER_H


#include "ShapeCollider.h"

class ConvexTriangleMeshCollider : public ShapeCollider
{
public:
    ConvexTriangleMeshCollider(GameObject *parent, std::vector<glm::vec3> vertices);

    //bool pointInside(const glm::vec3 &point);
    //bool raycast(const Ray &ray, RaycastResult &result);
    glm::vec3 getSupport(const glm::vec3 &dir);

private:
    std::vector<glm::vec3> vertices;
};

#endif // CONVEXTRIANGLEMESHCOLLIDER_H
