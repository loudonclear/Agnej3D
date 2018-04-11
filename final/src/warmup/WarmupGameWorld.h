#ifndef WARMUPGAMEWORLD_H
#define WARMUPGAMEWORLD_H

#include "engine/world/World.h"

class Transform;

class WarmupGameWorld : public World
{
public:
    WarmupGameWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    float elapsedTime;
    float fixedTickTime;
    int score1 = 0;
    int score2 = 0;
    float zoom = 3.0f;

    std::shared_ptr<Transform> playerTransform;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;
};

#endif // WARMUPGAMEWORLD_H
