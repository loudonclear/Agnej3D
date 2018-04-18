#ifndef FINALDEBUGWORLD_H
#define FINALDEBUGWORLD_H


#include "engine/world/World.h"

class FinalDebugWorld : public World
{
public:
    FinalDebugWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void onKeyPressed(QKeyEvent *event);


private:
    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;

    std::shared_ptr<Transform> shapeTransform;
    std::shared_ptr<GameObject> interaction;

    int shapeType = 0;
};

#endif // FINALDEBUGWORLD_H
