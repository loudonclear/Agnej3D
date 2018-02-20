#include "ChunkManager.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/BoxCollider.h"
#include "engine/graphics/Graphics.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/graphics/Camera.h"

glm::vec3 gravity = glm::vec3(0, -10, 0);

ChunkManager::ChunkManager(World* world, const int chunkx, const int chunky, const int chunkz, const int dimx, const int dimz) : m_world(world), chunkx(chunkx), chunky(chunky), chunkz(chunkz), dimx(dimx), dimz(dimz)
{
    Block AIR(true, true, 16.f / 256.f, glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0));
    Block GRASS(false, false, 16.f / 256.f, glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15));
    Block TOPGRASS(false, false, 16.f / 256.f, glm::vec2(3, 15), glm::vec2(3, 15), glm::vec2(1, 15), glm::vec2(0, 15), glm::vec2(3, 15), glm::vec2(3, 15));
    Block STONE(false, false, 16.f / 256.f, glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15));

    m_blockTypes.insert(std::pair<char, Block>(0, AIR));
    m_blockTypes.insert(std::pair<char, Block>(1, GRASS));
    m_blockTypes.insert(std::pair<char, Block>(2, TOPGRASS));
    m_blockTypes.insert(std::pair<char, Block>(3, STONE));

    m_chunks.reserve(dimx * dimz);
    m_noise = std::make_shared<PerlinNoise>(4);

    for (int i = 0; i < dimx; i++) {
        for (int j = 0; j < dimz; j++) {
            std::shared_ptr<GameObject> chunkObj = std::make_shared<GameObject>("chunk");
            std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkObj.get(), chunkx, chunky, chunkz);
            Graphics *g = Graphics::getGlobalInstance();
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsShapeComponent>(chunkObj.get(), g->getShape("sphere"), "terrain", true);
            std::shared_ptr<Transform> trans = std::make_shared<Transform>(chunkObj.get());

            Transform colliderScale(nullptr);
            colliderScale.scale(2.f*glm::vec3(chunkx, chunky, chunkz));
            std::shared_ptr<ShapeCollider> sc = std::make_shared<BoxCollider>(chunkObj.get(), colliderScale);
            trans->setPosition(glm::vec3(i * chunkx - 0.5f, -0.5f, j * chunkz - 0.5f));
            //trans->setScale(glm::vec3(chunkx, chunky, chunkz));

            for (int x = 0; x < chunkx; x++) {
                for (int z = 0; z < chunkz; z++) {
                    float res = m_noise->noise(i + x / (float) chunkx, j + z / (float) chunkz, 12.4f);
                    res = (res + 1) / 2.f;
                    int height = (int)(res * chunky);

                    int y = 0;
                    //chunk->setBlock(x, y++, z, m_blockTypes.find(2)->second);
                    for (; y < height - 8; y++) {
                        chunk->setBlock(x, y, z, m_blockTypes.find(3)->second);
                    }
                    for (; y < height - 1; y++) {
                        chunk->setBlock(x, y, z, m_blockTypes.find(1)->second);
                    }
                    chunk->setBlock(x, y++, z, m_blockTypes.find(2)->second);
                    for (; y < chunky; y++) {
                        chunk->setBlock(x, y, z, m_blockTypes.find(0)->second);
                    }

                }
            }

            chunkObj->addComponent(trans);
            chunkObj->addComponent(chunk);
            chunkObj->addComponent(gc);
            chunkObj->addComponent(sc);
            m_chunks.push_back(chunk);
            world->addGameObject("chunk" + std::to_string(i) + " " + std::to_string(j), chunkObj);
        }
    }

}

void ChunkManager::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<ShapeCollider> component = go->getComponent<ShapeCollider>();
    if (component != nullptr) m_components.push_back(component);
}

void ChunkManager::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<ShapeCollider> component = go->getComponent<ShapeCollider>();
    if (component != nullptr)
        m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}

void ChunkManager::update(float seconds) {

    for (auto iter = m_components.begin(); iter != m_components.end(); ++iter) {
        sweep(seconds, iter->get());
    }
}


bool ChunkManager::isPassable(int x, int y, int z) {
    if (x < 0 || y < 0 || z < 0) return true;

    int cx = x / chunkx;
    int cz = z / chunkz;

    if (cx >= dimx || cz >= dimz) return true;

    const int index = cx*dimz + cz;
    std::shared_ptr<Chunk> chunk = m_chunks[index];

    Block *block = chunk->getBlock(x % chunkx, y, z % chunkz);
    return block == nullptr ? true : block->passable;
}

void ChunkManager::sweep(float seconds, ShapeCollider *collider) {

    std::shared_ptr<RigidBody> rb = collider->getRigidBody();
    std::shared_ptr<Transform> t = collider->getTransform();
    glm::vec3 pos = collider->getCenter();
    glm::vec3 dim = glm::vec3(1, 2, 1) * 0.5f;

    glm::vec3 dist = seconds * rb->velocity;
    glm::ivec3 dir = glm::ivec3(dist.x >= 0 ? 1 : -1, dist.y >= 0 ? 1 : -1, dist.z >= 0 ? 1 : -1);

    glm::ivec3 minLocal = glm::ivec3(glm::round(pos - dim));
    glm::ivec3 maxLocal = glm::ivec3(glm::round(pos + dim));


    if (abs(dist.x) < abs(dist.z)) {
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
}
