#ifndef COLLISIONSHAPE_H
#define COLLISIONSHAPE_H

#include "limits"
#include "glm/glm.hpp"
#include <memory>

#include "Transform.h"
#include "Ray.h"
#include "RaycastResult.h"

const float FLOAT_EPSILON = 1e-4f;

class CollisionShape
{
public:
    CollisionShape() {}

    virtual bool raycast(const Ray &ray, std::shared_ptr<Transform> t, RaycastResult &result) = 0;
protected:

    bool SolveQuadratic(float a, float b, float c, float &t) {
        float discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0) return false;
        t = (-b - sqrt(discriminant)) / (2.0 * a);
        return (t > -FLOAT_EPSILON);
    }
};

#endif // COLLISIONSHAPE_H
