#ifndef WARMUPGAMESCREEN_H
#define WARMUPGAMESCREEN_H

#include "engine/Screen.h"

class Camera;

class WarmupGameScreen : public Screen
{
public:
    WarmupGameScreen();

    void tick(float seconds);
    void draw(Graphics *g);
    void resize(glm::vec2 &size);

    void onMousePressed(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseReleased(QMouseEvent *event);
    void onMouseWheelMoved(QWheelEvent *event);
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);

private:
    std::shared_ptr<Camera> m_camera;

    glm::vec3 m_velocity;
};

#endif // WARMUPGAMESCREEN_H
