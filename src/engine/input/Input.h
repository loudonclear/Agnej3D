#ifndef INPUT_H
#define INPUT_H

#include <map>
#include <QKeyEvent>
#include <glm/glm.hpp>

class Input
{
public:
    Input();

    static void onKeyPressed(QKeyEvent *event);
    static void onKeyReleased(QKeyEvent *event);
    static void onMousePressed(QMouseEvent *event);
    static void onMouseReleased(QMouseEvent *event);

    static bool isMouseButtonDown(int mouseButton);
    static bool isKeyDown(int key);

    static glm::ivec2 getMousePosition();
    static void setMousePosition(glm::ivec2 mp);
};

#endif // INPUT_H
