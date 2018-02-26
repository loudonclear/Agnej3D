#ifndef ENEMYCOLLIDER_H
#define ENEMYCOLLIDER_H


#include "engine/physics/BoxCollider.h"


class EnemyCollider : public BoxCollider
{
public:
    EnemyCollider(GameObject *parent);

    void onCollide(Collision::ContactData cd);
};
#endif // ENEMYCOLLIDER_H
