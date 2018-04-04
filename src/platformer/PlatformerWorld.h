#ifndef PLATFORMERWORLD_H
#define PLATFORMERWORLD_H


#include "engine/world/World.h"
#include "engine/world/NavMesh.h"

class RigidBody;
class OBJCollisionSystem;

class PlatformerWorld : public World
{
public:
    PlatformerWorld(std::string level, std::string levelNav);

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
    std::string m_level, m_levelNav;
    bool showNavMesh;
    std::shared_ptr<NavMesh> m_navMesh;

    OBJ::Triangle *start, *end;
    RaycastResult sr, er;
    std::shared_ptr<Transform> pt;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<CollisionSystem> m_collisionSystem;
    std::shared_ptr<InputSystem> m_inputSystem;

    std::shared_ptr<OBJCollisionSystem> m_objCollisionSystem;
};

#endif // PLATFORMERWORLD_H
