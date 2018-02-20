#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "engine/systems/System.h"
#include "glm/glm.hpp"
#include <vector>
#include "Chunk.h"
#include "engine/world/World.h"
#include "PerlinNoise.h"

class ChunkManager : public System
{
public:
    ChunkManager(World* world, const int chunkx, const int chunky, const int chunkz, const int dimx, const int dimz);

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void update(float seconds);
    bool isPassable(int x, int y, int z);

private:
    World *m_world;
    std::shared_ptr<PerlinNoise> m_noise;
    const int chunkx, chunky, chunkz, dimx, dimz;

    std::map<char, Block> m_blockTypes;
    std::vector<std::shared_ptr<Chunk>> m_chunks;

    std::vector<std::shared_ptr<ShapeCollider>> m_components;

    void sweep(float seconds, ShapeCollider *collider);
};

#endif // CHUNKMANAGER_H
