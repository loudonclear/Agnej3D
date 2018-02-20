#ifndef PLAYERMOVECOMPONENT_H
#define PLAYERMOVECOMPONENT_H

#include "engine/components/TickComponent.h"
#include "glm/glm.hpp"

class Transform;
class RigidBody;

class PlayerMoveComponent : public TickComponent
{
public:
    PlayerMoveComponent(GameObject *parent);
    void init();
    void tick(float seconds);

private:
    std::shared_ptr<Transform> m_transform;
    std::shared_ptr<RigidBody> m_rigidbody;
};

#endif // PLAYERMOVECOMPONENT_H
