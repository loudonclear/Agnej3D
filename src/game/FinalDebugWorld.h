#ifndef FINALDEBUGWORLD_H
#define FINALDEBUGWORLD_H


#include "engine/world/World.h"
#include "physics/volume/ConvexHull.h"
#include <random>

class RigidBody;

class FinalDebugWorld : public World
{
public:
    FinalDebugWorld(const std::vector<std::string> &shapes);

    void tick(float seconds);
    void draw(Graphics *g);
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

    std::vector<std::string> shapes;
    std::vector<std::shared_ptr<ConvexHull>> hulls;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

    std::shared_ptr<RigidBody> interaction;
    float interactionDist;
    bool dragging;
    int numObjects;
    float forceMult = 5.f;

    //std::shared_ptr<Transform> shapeTransform;
};

#endif // FINALDEBUGWORLD_H
