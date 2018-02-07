#ifndef CYLINDERCOLLIDER_H
#define CYLINDERCOLLIDER_H

#include "ShapeCollider.h"

class CylinderCollider : public ShapeCollider
{
public:
    CylinderCollider(std::shared_ptr<GameObject> &parent);

    //bool pointInside(const glm::vec3 &point);
    //bool raycast(const Ray &ray, RaycastResult &result);
    glm::vec3 getSupport(const glm::vec3 &dir);
};

#endif // CYLINDERCOLLIDER_H
