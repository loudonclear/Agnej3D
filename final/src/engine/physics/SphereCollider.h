#ifndef SPHERECOLLIDER_H
#define SPHERECOLLIDER_H

#include "ShapeCollider.h"

class SphereCollider : public ShapeCollider
{
public:
    SphereCollider(std::shared_ptr<GameObject> &parent);

    //virtual bool pointInside(const glm::vec3 &point);
    //virtual bool raycast(const Ray &ray, RaycastResult &result);

    glm::vec3 getSupport(const glm::vec3 &dir);
};

#endif // SPHERECOLLIDER_H
