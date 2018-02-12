#ifndef MINECRAFTMENUSCREEN_H
#define MINECRAFTMENUSCREEN_H


#include "engine/Screen.h"

class Camera;
class Graphics;

class MinecraftMenuScreen : public Screen
{
public:
    MinecraftMenuScreen();

    void tick(float seconds);
    void draw(Graphics *g);

    void onKeyPressed(QKeyEvent *event);

private:
    std::shared_ptr<Camera> m_camera;
};
#endif // MINECRAFTMENUSCREEN_H
