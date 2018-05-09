#ifndef CONVEXCOLLIDER_H
#define CONVEXCOLLIDER_H

#include "ConvexHull.h"
#include "physics/ShapeCollider.h"

class ConvexCollider : public ShapeCollider {
public:

    ConvexCollider(GameObject *parent, std::shared_ptr<ConvexHull> &hull, float mass);
    virtual ~ConvexCollider() {}

    void init();

    virtual glm::vec3 getSupportPointLocal(const glm::vec3 &dirWorldSpace) const override;
    virtual glm::vec3 getCenterOfMass() const override;
    virtual bool raycast(const Ray &ray, RaycastResult &res) const override;

protected:
    std::shared_ptr<ConvexHull> hull;

};

#endif // CONVEXCOLLIDER_H
