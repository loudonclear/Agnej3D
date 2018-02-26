#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "Component.h"
#include <QKeyEvent>
#include "glm/glm.hpp"

class InputComponent : public virtual Component
{
public:
    InputComponent(GameObject *parent);

    virtual void onKeyPressed(QKeyEvent *event) {}
    virtual void onKeyReleased(QKeyEvent *event) {}
    virtual void onMousePressed(QMouseEvent *event) {}
    virtual void onMouseReleased(QMouseEvent *event) {}
    virtual void onMouseMoved(glm::vec2 &delta) {}
    virtual void onMouseWheelMoved(QWheelEvent *event) {}
};

#endif // INPUTCOMPONENT_H
