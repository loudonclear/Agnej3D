#include "MinecraftWorld.h"

#include "MinecraftWorld.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/components/FixedTickComponent.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/graphics/Camera.h"
#include "minecraft/Chunk.h"
#include "minecraft/PerlinNoise.h"

MinecraftWorld::MinecraftWorld() : elapsedTime(0), fixedTickTime(1.f/60.f)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);


    Block AIR(true, true, 16.f / 256.f, glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0));
    Block GRASS(false, false, 16.f / 256.f, glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15));
    Block TOPGRASS(false, false, 16.f / 256.f, glm::vec2(3, 15), glm::vec2(3, 15), glm::vec2(1, 15), glm::vec2(0, 15), glm::vec2(3, 15), glm::vec2(3, 15));
    Block STONE(false, false, 16.f / 256.f, glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15));

    blockTypes.insert(std::pair<char, Block>(0, AIR));
    blockTypes.insert(std::pair<char, Block>(1, GRASS));
    blockTypes.insert(std::pair<char, Block>(2, TOPGRASS));
    blockTypes.insert(std::pair<char, Block>(3, STONE));


    PerlinNoise noise(4);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            std::shared_ptr<GameObject> chunkObj = std::make_shared<GameObject>("chunk");
            std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkObj, glm::vec3(32, 32, 32));
            Graphics *g = Graphics::getGlobalInstance();
            std::shared_ptr<GraphicsComponent> gc = std::make_shared<GraphicsShapeComponent>(chunkObj, g->getShape("sphere"), "terrain");
            std::shared_ptr<Transform> trans = std::make_shared<Transform>(chunkObj);
            trans->setPosition(glm::vec3(i * 32, 0, j * 32));

            for (unsigned int x = 0; x < 32; x++) {
                for (unsigned int z = 0; z < 32; z++) {
                    float res = noise.noise(i + x / 32.f, j + z / 32.f, 0.4f);
                    res = (res + 1) / 2.f;
                    int height = (int)(res * 32.f);

                    int y;
                    for (y = 0; y < height - 8; y++) {
                        chunk->setBlock(x, y, z, blockTypes.find(3)->second);
                    }
                    for (; y < height - 1; y++) {
                        chunk->setBlock(x, y, z, blockTypes.find(1)->second);
                    }
                    chunk->setBlock(x, y++, z, blockTypes.find(2)->second);
                    for (; y < 32; y++) {
                        chunk->setBlock(x, y, z, blockTypes.find(0)->second);
                    }

                }
            }

            chunkObj->addComponent(trans);
            chunkObj->addComponent(chunk);
            chunkObj->addComponent(gc);
            addGameObject("chunk" + std::to_string(i) + " " + std::to_string(j), chunkObj);
        }
    }


    init();
}


void MinecraftWorld::tick(float seconds) {
    elapsedTime += seconds;
    m_timingSystem->tick(seconds);

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);
        m_physicsSystem->update(fixedTickTime);

        elapsedTime -= fixedTickTime;
    }

    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    glm::vec3 look = c->getLook();
    glm::vec3 dir = glm::normalize(glm::vec3(look.x, look.y, look.z));
    glm::vec3 perp = glm::vec3(dir.z, 0, -dir.x);

    const float moveSpeed = 6.f;

    if (Input::isKeyDown(Qt::Key_W)) {
        c->translate(dir*seconds*moveSpeed);
    }
    if (Input::isKeyDown(Qt::Key_S)) {
        c->translate(-dir*seconds*moveSpeed);
    }
    if (Input::isKeyDown(Qt::Key_A)) {
        c->translate(perp*seconds*moveSpeed);
    }
    if (Input::isKeyDown(Qt::Key_D)) {
        c->translate(-perp*seconds*moveSpeed);
    }
}

void MinecraftWorld::draw(Graphics *g) {
    g->clearTransform();
    g->scale(glm::vec3(10, 1, 5));
    g->setMaterial("grass");
    g->drawShape("quad");

    m_graphicsSystem->draw(g);
}

void MinecraftWorld::onMouseMoved(glm::vec2 &delta) {
    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    c->rotate(-delta.x / 100.f, -delta.y / 100.f);
}
