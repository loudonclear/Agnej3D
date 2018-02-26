#ifndef MINECRAFTWORLD_H
#define MINECRAFTWORLD_H


#include "engine/world/World.h"
#include "engine/voxel/Chunk.h"

class RigidBody;
class ChunkManager;

class MinecraftWorld : public World
{
public:
    MinecraftWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void reset();

    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

    std::map<char, Block> blockTypes;

private:
    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<CollisionSystem> m_collisionSystem;
    std::shared_ptr<InputSystem> m_inputSystem;
    std::shared_ptr<ChunkManager> m_chunkManager;

    float zoom = 3.0f;

    glm::ivec3 position;
    glm::vec3 normal;
    bool visualBlock;
};

#endif // MINECRAFTWORLD_H
