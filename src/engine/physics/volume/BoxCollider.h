#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "physics/ShapeCollider.h"

class BoxCollider : public ShapeCollider {
public:

    BoxCollider(GameObject *parent, float mass);
    virtual ~BoxCollider() {}

    void init();

    virtual glm::vec3 getSupportPointLocal(const glm::vec3 &dirWorldSpace) const override;
    virtual glm::vec3 getCenterOfMass() const override;
    virtual bool raycast(const Ray &ray, RaycastResult &res) const override;

};

#endif // BOXCOLLIDER_H
