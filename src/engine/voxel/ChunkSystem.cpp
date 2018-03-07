#include "ChunkSystem.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/BoxCollider.h"
#include "engine/graphics/Graphics.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/input/Input.h"

const int chunkRadius = 5;

ChunkSystem::ChunkSystem(World* world, std::map<char, Block> *blockTypes, std::function<int(int, int)> heightFunction, const int chunkx, const int chunky, const int chunkz) : m_world(world), m_blockTypes(blockTypes), m_heightFunction(heightFunction), chunkx(chunkx), chunky(chunky), chunkz(chunkz)
{
    m_noise = std::make_shared<PerlinNoise>(4);
    lastPlayerChunk = glm::ivec3(-1);
}


void ChunkSystem::addChunk(int i, int j) {
    if (m_world->getGameObject("chunk" + std::to_string(i) + " " + std::to_string(j)) != nullptr) return;
    const float roughness = 1.5f;

    std::shared_ptr<GameObject> chunkObj = std::make_shared<GameObject>("chunk");
    std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkObj.get(), chunkx, chunky, chunkz);
    Graphics *g = Graphics::getGlobalInstance();
    std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsShapeComponent>(chunkObj.get(), g->getShape("sphere"), "terrain", true);
    std::shared_ptr<Transform> trans = std::make_shared<Transform>(chunkObj.get());

    Transform colliderScale(nullptr);
    colliderScale.scale(2.f*glm::vec3(chunkx, chunky, chunkz));
    std::shared_ptr<ShapeCollider> sc = std::make_shared<BoxCollider>(chunkObj.get(), colliderScale);
    trans->setPosition(glm::vec3(i * chunkx - 0.5f, -0.5f, j * chunkz - 0.5f));

    for (int x = 0; x < chunkx; x++) {
        for (int z = 0; z < chunkz; z++) {
            float res = m_noise->noise(roughness * (i + x / (float) chunkx), roughness * (j + z / (float) chunkz), 0.4f);
            res = (res + 1) / 2.f;
            int height = (int)(res * chunky);

            for (int y = 0; y < chunky; y++) {
                chunk->setBlock(x, y, z, m_blockTypes->find(m_heightFunction(y, height))->second);
            }
        }
    }

    chunkObj->addComponent(trans);
    chunkObj->addComponent(chunk);
    chunkObj->addComponent(gc);
    chunkObj->addComponent(sc);
    m_world->addGameObject("chunk" + std::to_string(i) + " " + std::to_string(j), chunkObj);
}

std::shared_ptr<Chunk> ChunkSystem::getChunk(int i, int j) {
    std::shared_ptr<GameObject> chunkObj = m_world->getGameObject("chunk" + std::to_string(i) + " " + std::to_string(j));
    if (chunkObj == nullptr) return nullptr;

    return chunkObj->getComponent<Chunk>();
}

void ChunkSystem::removeChunk(int i, int j) {
    m_world->removeGameObject("chunk" + std::to_string(i) + " " + std::to_string(j));
}


void ChunkSystem::update(const glm::vec3 &playerPosition, const bool loadAll) {

    if (!chunksToAdd.empty()) {
        std::pair<int, int> chunkPos = *chunksToAdd.begin();
        addChunk(chunkPos.first, chunkPos.second);
        chunksToAdd.erase(chunkPos);
    }

    glm::ivec3 playerChunk = glm::ivec3(playerPosition.x / chunkx, playerPosition.y / chunky, playerPosition.z / chunkz);
    if (playerPosition.x < 0) playerChunk.x -= 1;
    if (playerPosition.z < 0) playerChunk.z -= 1;

    //std::cout << glm::to_string(playerChunk) << std::endl;

    if (playerChunk.x != lastPlayerChunk.x || playerChunk.z != lastPlayerChunk.z) {

       for (int i = lastPlayerChunk.x - chunkRadius - 1; i <= lastPlayerChunk.x + chunkRadius + 1; i++) {
           for (int j = lastPlayerChunk.z - chunkRadius - 1; j <= lastPlayerChunk.z + chunkRadius + 1; j++) {
               if (abs(playerChunk.x - i) > chunkRadius + 1 || abs(playerChunk.z - j) > chunkRadius + 1) {
                   removeChunk(i, j);
               }
           }
       }

       for (int i = playerChunk.x - chunkRadius; i <= playerChunk.x + chunkRadius; i++) {
           for (int j = playerChunk.z - chunkRadius; j <= playerChunk.z + chunkRadius; j++) {
               chunksToAdd.emplace(std::pair<int, int>(i, j));
           }
       }
    }

    lastPlayerChunk = playerChunk;

    if (loadAll) {
        while (!chunksToAdd.empty()) {
            std::pair<int, int> chunkPos = *chunksToAdd.begin();
            addChunk(chunkPos.first, chunkPos.second);
            chunksToAdd.erase(chunkPos);
        }
    }
}


bool ChunkSystem::isPassable(int x, int y, int z) {
    Block *block = getBlock(x, y, z);
    return block == nullptr ? true : block->passable;
}

Block* ChunkSystem::getBlock(int x, int y, int z) {
    int cx = x < 0 ? (x + 1) / chunkx - 1 : x / chunkx;
    int cz = z < 0 ? (z + 1) / chunkz - 1 : z / chunkz;

    std::shared_ptr<Chunk> chunk = getChunk(cx, cz);
    if (chunk == nullptr) return nullptr;

    int newx = (x < 0) ? x - (cx * chunkx) : x % chunkx;
    int newz = (z < 0) ? z - (cz * chunkz) : z % chunkz;

    return chunk->getBlock(newx, y, newz);
}

void ChunkSystem::setBlock(int x, int y, int z, Block& block) {
    int cx = x < 0 ? (x + 1) / chunkx - 1 : x / chunkx;
    int cz = z < 0 ? (z + 1) / chunkz - 1 : z / chunkz;

    std::shared_ptr<Chunk> chunk = getChunk(cx, cz);
    if (chunk == nullptr) return;

    int newx = (x < 0) ? x - (cx * chunkx) : x % chunkx;
    int newz = (z < 0) ? z - (cz * chunkz) : z % chunkz;

    chunk->setBlock(newx, y, newz, block);
    chunk->update();
}

bool ChunkSystem::raycast(const glm::vec3 &o, const glm::vec3 &d, const float maxt, glm::ivec3 &position, glm::vec3 &normal) {
    const glm::vec3 r = glm::normalize(d);
    const glm::ivec3 dir = glm::ivec3(glm::sign(r));

    glm::ivec3 pos = glm::ivec3(glm::round(o));

    const glm::vec3 delta = glm::vec3(r.x == 0.f ? 0 : fabs(1.f / r.x),
                                      r.y == 0.f ? 0 : fabs(1.f / r.y),
                                      r.z == 0.f ? 0 : fabs(1.f / r.z));

    glm::vec3 tmax = glm::vec3(r.x == 0.f ? FLT_MAX : (pos.x + dir.x - o.x) / r.x,
                               r.y == 0.f ? FLT_MAX : (pos.y + dir.y - o.y) / r.y,
                               r.z == 0.f ? FLT_MAX : (pos.z + dir.z - o.z) / r.z);


    Block* block = getBlock(pos.x, pos.y, pos.z);

    int count = 0;
    int index;
    while (block == nullptr || block->passable) {
        if (tmax.x < tmax.y) {
            if (tmax.x < tmax.z) {
                tmax.x += delta.x;
                pos.x += dir.x;
                index = 0;
            } else {
                tmax.z += delta.z;
                pos.z += dir.z;
                index = 2;
            }
        } else {
            if (tmax.y < tmax.z) {
                tmax.y += delta.y;
                pos.y += dir.y;
                index = 1;
            } else {
                tmax.z += delta.z;
                pos.z += dir.z;
                index = 2;
            }
        }

        if (count++ >= maxt) return false;

        block = getBlock(pos.x, pos.y, pos.z);
    }

    position = pos;
    normal = glm::vec3(0, 0, 0);
    if (index == 0) normal.x = -dir.x;
    else if (index == 1) normal.y = -dir.y;
    else if (index == 2) normal.z = -dir.z;

    return true;
}

void ChunkSystem::sweep(float seconds, std::shared_ptr<ShapeCollider> collider) {

    std::shared_ptr<RigidBody> rb = collider->getRigidBody();
    if (rb == nullptr) return;
    std::shared_ptr<Transform> t = collider->getTransform();
    glm::vec3 pos = collider->getCenter();
    glm::vec3 dim = collider->getTransform()->getScale() * 0.5f;

    glm::vec3 dist = seconds * rb->velocity;
    glm::ivec3 dir = glm::ivec3(dist.x >= 0 ? 1 : -1, dist.y >= 0 ? 1 : -1, dist.z >= 0 ? 1 : -1);

    glm::ivec3 minLocal = glm::ivec3(glm::round(pos - dim));
    glm::ivec3 maxLocal = glm::ivec3(glm::round(pos + dim));


    if (fabs(dist.x) < fabs(dist.z)) {
        int startX = glm::round(pos.x + dim.x * dir.x);
        int endX = glm::round(dist.x + pos.x + dim.x * dir.x);

        for (int x = startX; x != endX + dir.x; x += dir.x) {
            for (int z = minLocal.z; z <= maxLocal.z; z++) {
                for (int y = minLocal.y; y <= maxLocal.y; y++) {

                    if (!isPassable(x, y, z)) {
                        t->setPosition(glm::vec3(x - dir.x * 0.5001f - dim.x * dir.x, pos.y, pos.z));
                        rb->velocity.x = 0;
                    }
                }
            }
        }

        int startZ = glm::round(pos.z + dim.z * dir.z);
        int endZ = glm::round(dist.z + pos.z + dim.z * dir.z);

        for (int z = startZ; z != endZ + dir.z; z += dir.z) {
            for (int y = minLocal.y; y <= maxLocal.y; y++) {
                for (int x = minLocal.x; x <= maxLocal.x; x++) {

                    if (!isPassable(x, y, z)) {
                        t->setPosition(glm::vec3(pos.x, pos.y, z - dir.z * 0.5001f - dim.z * dir.z));
                        rb->velocity.z = 0;
                    }
                }
            }
        }
    } else {
        int startZ = glm::round(pos.z + dim.z * dir.z);
        int endZ = glm::round(dist.z + pos.z + dim.z * dir.z);

        for (int z = startZ; z != endZ + dir.z; z += dir.z) {
            for (int y = minLocal.y; y <= maxLocal.y; y++) {
                for (int x = minLocal.x; x <= maxLocal.x; x++) {

                    if (!isPassable(x, y, z)) {
                        t->setPosition(glm::vec3(pos.x, pos.y, z - dir.z * 0.5001f - dim.z * dir.z));
                        rb->velocity.z = 0;
                    }
                }
            }
        }


        int startX = glm::round(pos.x + dim.x * dir.x);
        int endX = glm::round(dist.x + pos.x + dim.x * dir.x);

        for (int x = startX; x != endX + dir.x; x += dir.x) {
            for (int z = minLocal.z; z <= maxLocal.z; z++) {
                for (int y = minLocal.y; y <= maxLocal.y; y++) {

                    if (!isPassable(x, y, z)) {
                        t->setPosition(glm::vec3(x - dir.x * 0.5001f - dim.x * dir.x, pos.y, pos.z));
                        rb->velocity.x = 0;
                    }
                }
            }
        }

    }

    int startY = glm::round(pos.y + dim.y * dir.y);
    int endY = glm::round(dist.y + pos.y + dim.y * dir.y);

    for (int y = startY; y != endY + dir.y; y += dir.y) {
        for (int z = minLocal.z; z <= maxLocal.z; z++) {
            for (int x = minLocal.x; x <= maxLocal.x; x++) {

                if (!isPassable(x, y, z)) {
                    t->setPosition(glm::vec3(pos.x, y - dir.y * 0.5001f - dim.y * dir.y, pos.z));
                    rb->velocity.y = 0;
                    rb->isGrounded = true;
                }
            }
        }
    }


    rb->integrateVelocities(seconds);
    rb->integrateForces(seconds);
}


