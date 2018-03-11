#ifndef MINECRAFTMENUSCREEN_H
#define MINECRAFTMENUSCREEN_H

#include "engine/systems/TimingSystem.h"
#include "engine/systems/GraphicsSystem.h"
#include "engine/Screen.h"
#include "minecraft/MenuWorld.h"

class Camera;
class Graphics;

class MinecraftMenuScreen : public Screen
{
public:
    MinecraftMenuScreen();

    void tick(float seconds);
    void draw(Graphics *g);

    void onKeyPressed(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<MenuWorld> m_world;

};
#endif // MINECRAFTMENUSCREEN_H
