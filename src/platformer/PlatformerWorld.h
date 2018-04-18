#ifndef PLATFORMERWORLD_H
#define PLATFORMERWORLD_H


#include "engine/world/World.h"

class RigidBody;

class PlatformerWorld : public World
{
public:
    PlatformerWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void reset();

    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;
    std::shared_ptr<InputSystem> m_inputSystem;
};

#endif // PLATFORMERWORLD_H
