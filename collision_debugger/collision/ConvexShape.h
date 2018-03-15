#ifndef CONVEXSHAPE_H
#define CONVEXSHAPE_H

#include "CollisionShape.h"

class ConvexShape : public CollisionShape
{
public:
    ConvexShape() {}
    virtual glm::vec3 getSupport(const glm::vec3 &dir, std::shared_ptr<Transform> t) = 0;
    virtual bool pointInside(const glm::vec3 &point, std::shared_ptr<Transform> t) = 0;
};

#endif // CONVEXSHAPE_H
