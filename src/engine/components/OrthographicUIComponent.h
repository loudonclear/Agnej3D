#ifndef ORTHOGRAPHICUICOMPONENT_H
#define ORTHOGRAPHICUICOMPONENT_H


#include "engine/components/UIComponent.h"

class OrthographicUIComponent : public virtual UIComponent
{
public:
    OrthographicUIComponent(GameObject *parent, const std::string &materialName);

    virtual void drawBegin(Graphics *g);
    virtual void drawEnd(Graphics *g);

private:
    glm::vec2 screenSize;
};

#endif // ORTHOGRAPHICUICOMPONENT_H
