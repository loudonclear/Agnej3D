#ifndef SCREEN_H
#define SCREEN_H

#include "engine/util/CommonIncludes.h"
#include <QKeyEvent>
#include "Application.h"

class Graphics;

class Screen
{
public:
    Screen();

    void setApplication(Application* app);
    Application* getApplication();

    virtual void tick(float seconds) {}
    virtual void draw(Graphics *g) {}
    virtual void resize(glm::vec2 &size) {}

    virtual void onMousePressed(QMouseEvent *event) {}
    virtual void onMouseMoved(glm::vec2 &delta) {}
    virtual void onMouseReleased(QMouseEvent *event) {}
    virtual void onMouseWheelMoved(QWheelEvent *event) {}
    virtual void onKeyPressed(QKeyEvent *event) {}
    virtual void onKeyReleased(QKeyEvent *event) {}

protected:
    Application* application;
};

#endif // SCREEN_H
