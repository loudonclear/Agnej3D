#ifndef TICKCOMPONENT_H
#define TICKCOMPONENT_H

#include "Component.h"

class TickComponent : public Component
{
public:
    TickComponent(GameObject *parent);

    virtual void tick(float seconds) {}
};

#endif // TICKCOMPONENT_H
