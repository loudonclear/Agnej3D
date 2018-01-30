#ifndef WARMUPMENUSCREEN_H
#define WARMUPMENUSCREEN_H

#include "engine/Screen.h"

class Camera;
class Graphics;

class WarmupMenuScreen : public Screen
{
public:
    WarmupMenuScreen();

    void draw(Graphics *g);
    void onKeyPressed(QKeyEvent *event);

private:
    std::shared_ptr<Camera> m_camera;
};

#endif // WARMUPMENUSCREEN_H
