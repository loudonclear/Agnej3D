#ifndef WARMUPDEBUGWORLD_H
#define WARMUPDEBUGWORLD_H


#include "engine/world/World.h"

class WarmupDebugWorld : public World
{
public:
    WarmupDebugWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void onKeyPressed(QKeyEvent *event);


private:
    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<CollisionSystem> m_physicsSystem;

    std::shared_ptr<Transform> shapeTransform;
    std::shared_ptr<GameObject> interaction;

    int shapeType = 0;
};

#endif // WARMUPDEBUGWORLD_H
