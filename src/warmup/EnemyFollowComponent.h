#ifndef ENEMYFOLLOWCOMPONENT_H
#define ENEMYFOLLOWCOMPONENT_H

#include "engine/components/TickComponent.h"

class Transform;


class EnemyFollowComponent : public TickComponent
{
public:
    EnemyFollowComponent(std::shared_ptr<GameObject> &parent);

    void tick(float seconds);
    void init();

private:
    std::shared_ptr<Transform> m_target, m_transform;
};

#endif // ENEMYFOLLOWCOMPONENT_H
