#ifndef SHAPECOLLIDER_H
#define SHAPECOLLIDER_H

#include "components/Component.h"
#include <vector>
#include <glm/glm.hpp>
#include "pool_vector.h"
#include "Ray.h"
#include "RaycastResult.h"

class PhysicalMaterial;
class RigidBody;
class Transform;

#define SIGN(n) ((n)<0 ? -1:1)
const float FLOAT_EPSILON = 0.0001f;

class ShapeCollider : public virtual Component {

public:
    ShapeCollider(GameObject *parent);
    virtual ~ShapeCollider() {}
    void onInvalidate();

    void init();

    std::shared_ptr<RigidBody> getRigidBody() { return rigidBody; }
    std::shared_ptr<Transform> getTransform() { return transform; }

	bool isStatic() const;
	bool isDynamic() const;

    virtual glm::vec3 getSupportPointLocal(const glm::vec3 &dirWorldSpace) const = 0;
    virtual glm::vec3 getCenterOfMass() const = 0;
    virtual bool raycast(const Ray &ray, RaycastResult &res) const = 0;

    void* cluster;
    pool_vector<class Contact*> contactInvolvement;

    float extent;
    float mass;
    float inverseMass;
    glm::mat4x4 inertialTensor;
    glm::mat4x4 invInertialTensor;
    float restitution;
    float friction;

protected:
    glm::vec3 _worldToLocal(const glm::vec3 &dirWorldSpace) const;

    inline float Min(const glm::vec3 &a) const { return std::min(std::min(a[0], a[1]), a[2]); }
    inline float Max(const glm::vec3 &a) const { return std::max(std::max(a[0], a[1]), a[2]); }

    inline glm::vec3 Min(const glm::vec3 &a, const glm::vec3 &b) const
        { return glm::vec3(std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2])); }
    inline glm::vec3 Max(const glm::vec3 &a, const glm::vec3 &b) const
        { return glm::vec3(std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2])); }

    std::shared_ptr<Transform> transform;
    std::shared_ptr<RigidBody> rigidBody;
};

#endif // SHAPECOLLIDER_H
