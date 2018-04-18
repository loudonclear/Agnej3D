#ifndef PLATFORMERSCREEN_H
#define PLATFORMERSCREEN_H


#include "engine/Screen.h"

class Camera;
class World;

class PlatformerScreen : public Screen
{
public:
    PlatformerScreen();

    static std::shared_ptr<Camera> camera;

    void tick(float seconds);
    void draw(Graphics *g);
    void resize(glm::vec2 &size);

    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    std::shared_ptr<World> m_world;
};

#endif // PLATFORMERSCREEN_H
