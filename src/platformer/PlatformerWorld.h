#ifndef PLATFORMERWORLD_H
#define PLATFORMERWORLD_H


#include "engine/world/World.h"

class RigidBody;
class OBJCollisionSystem;

class PlatformerWorld : public World
{
public:
    PlatformerWorld(std::string level);

    void tick(float seconds);
    void draw(Graphics *g);
    void reset();

    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    float elapsedTime;
    float fixedTickTime;
    std::string m_level;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<CollisionSystem> m_collisionSystem;
    std::shared_ptr<InputSystem> m_inputSystem;

    std::shared_ptr<OBJCollisionSystem> m_objCollisionSystem;
};

#endif // PLATFORMERWORLD_H
