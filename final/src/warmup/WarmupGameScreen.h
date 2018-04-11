#ifndef WARMUPGAMESCREEN_H
#define WARMUPGAMESCREEN_H

#include "engine/Screen.h"

class Camera;
class World;

class WarmupGameScreen : public Screen
{
public:
    WarmupGameScreen();

    static std::shared_ptr<Camera> camera;

    void tick(float seconds);
    void draw(Graphics *g);
    void resize(glm::vec2 &size);

    void onKeyPressed(QKeyEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:

    std::shared_ptr<World> m_world;
};

#endif // WARMUPGAMESCREEN_H
