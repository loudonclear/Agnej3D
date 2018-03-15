#ifndef SPHERESHAPE_H
#define SPHERESHAPE_H

#include "ConvexShape.h"

class SphereShape : public ConvexShape
{
public:
    SphereShape();

    virtual glm::vec3 getSupport(const glm::vec3 &dir, std::shared_ptr<Transform> t);
    virtual bool pointInside(const glm::vec3 &point, std::shared_ptr<Transform> t);
    virtual bool raycast(const Ray &ray, std::shared_ptr<Transform> t, RaycastResult &result);
};

#endif // SPHERESHAPE_H
