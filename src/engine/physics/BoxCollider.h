#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "ShapeCollider.h"

class BoxCollider : public ShapeCollider
{
public:
    BoxCollider(GameObject *parent, Transform colliderTransform = Transform(nullptr));

    bool pointInside(const glm::vec3 &point);
//    bool raycast(const Ray &ray, RaycastResult &result);
    glm::vec3 getSupport(const glm::vec3 &dir);

};

#endif // BOXCOLLIDER_H
