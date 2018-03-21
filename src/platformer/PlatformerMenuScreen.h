#ifndef PLATFORMERMENUSCREEN_H
#define PLATFORMERMENUSCREEN_H

#include "engine/systems/TimingSystem.h"
#include "engine/systems/GraphicsSystem.h"
#include "engine/Screen.h"
#include "minecraft/MenuWorld.h"

class Camera;
class Graphics;

class PlatformerMenuScreen : public Screen
{
public:
    PlatformerMenuScreen();

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


#endif // PLATFORMERMENUSCREEN_H
