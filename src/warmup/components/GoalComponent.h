#ifndef GOALCOMPONENT_H
#define GOALCOMPONENT_H

#include "engine/physics/BoxCollider.h"


class GoalComponent : public BoxCollider
{
public:
    GoalComponent(GameObject *parent, int &score);

    void onCollide(Collision::ContactData cd);

private:
    int &score;
};

#endif // GOALCOMPONENT_H
