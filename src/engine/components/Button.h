#ifndef BUTTON_H
#define BUTTON_H

#include "TickComponent.h"
#include "InputComponent.h"
#include "HUDComponent.h"

class Button : public virtual TickComponent, public virtual InputComponent, public virtual HUDComponent
{
public:
    Button(GameObject *parent, const std::string &materialName, glm::vec2 offset, glm::vec2 length);

    virtual void tick(float seconds);
    virtual void draw(Graphics *g);
    virtual void onMouseReleased(QMouseEvent *event);

    virtual void onClick() = 0;

private:

    glm::vec2 m_offset;
    glm::vec2 m_length;
    bool canClick;
};

#endif // BUTTON_H
