#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "engine/components/Button.h"

#include "engine/components/TickComponent.h"
#include "engine/components/InputComponent.h"
#include "engine/components/HUDComponent.h"

class Application;

class MenuButton : public virtual TickComponent, public virtual InputComponent, public virtual HUDComponent
{
public:
    MenuButton(GameObject *parent, std::string text, const std::string &materialName, glm::vec2 offset, glm::vec2 length);

    virtual void tick(float seconds);
    virtual void draw(Graphics *g);
    virtual void onMouseReleased(QMouseEvent *event);

    virtual void onClick();

private:

    std::string m_text;
    glm::vec2 m_offset;
    glm::vec2 m_length;
    bool canClick;
};

#endif // MENUBUTTON_H
