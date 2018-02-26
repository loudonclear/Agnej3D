#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "engine/systems/System.h"
#include "glm/glm.hpp"
#include <vector>
#include "Chunk.h"
#include "engine/world/World.h"
#include "PerlinNoise.h"
#include <set>

class ChunkManager : public System
{
public:
    ChunkManager(World* world, std::map<char, Block> *blockTypes, std::function<int(int, int)> heightFunction, const int chunkx, const int chunky, const int chunkz);

    void addGameObject(const std::shared_ptr<GameObject> &go) {}
    void removeGameObject(const std::shared_ptr<GameObject> &go) {}

    void addChunk(int i, int j);
    std::shared_ptr<Chunk> getChunk(int i, int j);
    void removeChunk(int i, int j);

    void sweep(float seconds, std::shared_ptr<ShapeCollider> collider);

    Block* getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, Block& block);
    bool raycast(const glm::vec3 &o, const glm::vec3 &d, const float maxt, glm::ivec3 &position, glm::vec3 &normal);

    void update(const glm::vec3 &playerPosition, const bool loadAll = false);
    bool isPassable(int x, int y, int z);

    std::map<char, Block> *getBlockTypes() { return m_blockTypes; }

private:
    World *m_world;
    std::shared_ptr<PerlinNoise> m_noise;
    std::map<char, Block> *m_blockTypes;
    std::function<int(int, int)> m_heightFunction;

    glm::ivec3 lastPlayerChunk;
    std::set<std::pair<int,int>> chunksToAdd;

    const int chunkx, chunky, chunkz;
};

#endif // CHUNKMANAGER_H
