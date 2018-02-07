#ifndef WARMUPDEBUGSCREEN_H
#define WARMUPDEBUGSCREEN_H


#include "engine/Screen.h"

class Camera;
class World;

class WarmupDebugScreen : public Screen
{
public:
    WarmupDebugScreen();

    void tick(float seconds);
    void draw(Graphics *g);
    void resize(glm::vec2 &size);

    void onKeyPressed(QKeyEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<World> m_world;
};


#endif // WARMUPDEBUGSCREEN_H
