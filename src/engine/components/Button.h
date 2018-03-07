#ifndef BUTTON_H
#define BUTTON_H

#include "TickComponent.h"
#include "InputComponent.h"
#include "HUDComponent.h"

class Button : public virtual TickComponent, public virtual InputComponent, public virtual HUDComponent
{
public:
    Button(GameObject *parent, const std::string &materialName, glm::ivec2 offset, glm::ivec2 length);

    void tick(float seconds);
    void draw(Graphics *g);

    virtual void onClick();

private:

    glm::ivec2 m_offset;
    glm::ivec2 m_length;
};

#endif // BUTTON_H
