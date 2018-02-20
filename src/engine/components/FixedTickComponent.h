#ifndef FIXEDTICKCOMPONENT_H
#define FIXEDTICKCOMPONENT_H

#include "Component.h"

class FixedTickComponent : public Component
{
public:
    FixedTickComponent(GameObject *parent);

    virtual void fixedTick(float seconds) {}
};

#endif // FIXEDTICKCOMPONENT_H
