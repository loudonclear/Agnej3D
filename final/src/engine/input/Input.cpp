#include "Input.h"

Input::Input()
{

}

static std::map<int, bool> keys;
static std::map<int, bool> mouse;

void Input::onKeyPressed(QKeyEvent *event) {
    keys[event->key()] = true;
}

void Input::onKeyReleased(QKeyEvent *event) {
    keys[event->key()] = false;
}

void Input::onMousePressed(QMouseEvent *event) {
    mouse[event->button()] = true;
}

void Input::onMouseReleased(QMouseEvent *event) {
    mouse[event->button()] = false;
}

bool Input::isMouseButtonDown(int mouseButton) {
    return mouse[mouseButton];
}

bool Input::isKeyDown(int key) {
    return keys[key];
}
