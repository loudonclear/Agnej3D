#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H


#include "engine/world/World.h"
#include "minecraft/Chunk.h"

class RigidBody;
class ChunkManager;

class MinecraftWorld : public World
{
public:
    MinecraftWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

private:
    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;
    std::shared_ptr<ChunkManager> m_chunkManager;

    float zoom = 3.0f;
};

#endif // MINECRAFTWORLD_H
