#ifndef HUDCOMPONENT_H
#define HUDCOMPONENT_H


#include "engine/components/UIComponent.h"

class HUDComponent : public virtual UIComponent
{
public:
    HUDComponent(GameObject *parent, const std::string &materialName);

    virtual void drawBegin(Graphics *g);
    virtual void drawEnd(Graphics *g);
};
#endif // HUDCOMPONENT_H
