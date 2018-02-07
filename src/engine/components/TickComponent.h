#ifndef TICKCOMPONENT_H
#define TICKCOMPONENT_H

#include "Component.h"

class TickComponent : public Component
{
public:
    TickComponent(std::shared_ptr<GameObject> &parent);

    virtual void tick(float seconds) {}
};

#endif // TICKCOMPONENT_H
