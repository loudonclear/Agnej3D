#ifndef TICKCOMPONENT_H
#define TICKCOMPONENT_H

#include "Component.h"

class TickComponent : public virtual Component
{
public:
    TickComponent(GameObject *parent) : Component(parent) {}

    virtual void tick(float seconds) {}

    virtual void fixedTick(float seconds) {}

    virtual void lateTick(float seconds) {}
};

#endif // TICKCOMPONENT_H
