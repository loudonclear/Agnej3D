#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "System.h"
#include "engine/components/InputComponent.h"

class InputSystem : public System
{
public:
    InputSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:

    std::vector<std::shared_ptr<InputComponent>> m_components;
};

#endif // INPUTSYSTEM_H
