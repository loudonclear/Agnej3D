#ifndef PLAYERMOVECOMPONENT_H
#define PLAYERMOVECOMPONENT_H

#include "engine/physics/ShapeCollider.h"
#include "engine/components/TickComponent.h"
#include "engine/components/InputComponent.h"
#include "glm/glm.hpp"

class Transform;
class Camera;
class RigidBody;
class OBJCollisionSystem;

class PlayerMoveComponent : public virtual TickComponent, public virtual InputComponent
{
public:
    PlayerMoveComponent(GameObject *parent, std::shared_ptr<OBJCollisionSystem> objc);
    void init();

    void tick(float seconds);
    void fixedTick(float seconds);
    void lateTick(float seconds);

    void onMouseReleased(QMouseEvent *event) {}
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    std::shared_ptr<Transform> m_transform;
    std::shared_ptr<RigidBody> m_rigidbody;
    std::shared_ptr<ShapeCollider> m_shapeCollider;

    std::shared_ptr<OBJCollisionSystem> m_objCollisionSystem;

    std::shared_ptr<Camera> m_camera;

    float zoom = 3.0f;
};

#endif // PLAYERMOVECOMPONENT_H
