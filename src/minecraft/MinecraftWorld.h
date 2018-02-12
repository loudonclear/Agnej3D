#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H


#include "engine/world/World.h"
#include "minecraft/Chunk.h"

class Transform;

class MinecraftWorld : public World
{
public:
    MinecraftWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void onMouseMoved(glm::vec2 &delta);

private:
    float elapsedTime;
    float fixedTickTime;

    std::map<char, Block> blockTypes;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;
};

#endif // MINECRAFTWORLD_H
