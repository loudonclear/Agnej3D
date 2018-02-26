#include "MinecraftWorld.h"

#include "MinecraftWorld.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/physics/RigidBody.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/graphics/Camera.h"
#include "engine/voxel/ChunkManager.h"
#include "engine/voxel/PerlinNoise.h"
#include "minecraft/Enemy.h"
#include "minecraft/Player.h"
#include <QCoreApplication>
#include <QApplication>

int heightFunction(int y, int height){
    if (y < height - 8) {
        return 3;
    }
    else if (y < height - 1) {
        return 1;
    }
    else if (y == height - 1) {
        return 2;
    }
    else {
        return 0;
    }
}

MinecraftWorld::MinecraftWorld() : elapsedTime(0), fixedTickTime(1.f/60.f) {
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_collisionSystem = std::make_shared<CollisionSystem>();
    m_inputSystem = std::make_shared<InputSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_collisionSystem);
    addSystem(m_inputSystem);


    Block AIR(true, true, 16.f / 256.f, glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0), glm::vec2(0, 0));
    Block GRASS(false, false, 16.f / 256.f, glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15), glm::vec2(2, 15));
    Block TOPGRASS(false, false, 16.f / 256.f, glm::vec2(3, 15), glm::vec2(3, 15), glm::vec2(2, 15), glm::vec2(0, 15), glm::vec2(3, 15), glm::vec2(3, 15));
    Block STONE(false, false, 16.f / 256.f, glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15), glm::vec2(1, 15));

    blockTypes.insert(std::pair<char, Block>(0, AIR));
    blockTypes.insert(std::pair<char, Block>(1, GRASS));
    blockTypes.insert(std::pair<char, Block>(2, TOPGRASS));
    blockTypes.insert(std::pair<char, Block>(3, STONE));

    m_chunkManager = std::make_shared<ChunkManager>(this, &blockTypes, heightFunction, 32, 32, 32);
    addSystem(m_chunkManager);


    std::shared_ptr<GameObject> player = std::make_shared<Player>("player");
    addGameObject("player", player, false);

    std::shared_ptr<GameObject> enemy = std::make_shared<Enemy>("enemy", glm::vec3(20, 30, 20));
    addGameObject("enemy", enemy, false);

    std::shared_ptr<GameObject> enemy2 = std::make_shared<Enemy>("enemy2", glm::vec3(10, 30, 20));
    addGameObject("enemy2", enemy2, false);


    init();
    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    m_chunkManager->update(c->getEye(), true);
}


void MinecraftWorld::tick(float seconds) {
    elapsedTime += seconds;

    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;

    m_timingSystem->tick(seconds);
    m_chunkManager->update(c->getEye());

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);

        m_collisionSystem->update(fixedTickTime);
        elapsedTime -= fixedTickTime;
    } 


    if (m_chunkManager->raycast(c->getEye(), c->getLook(), 20, position, normal)) visualBlock = true;
    else visualBlock = false;

    m_timingSystem->lateTick(seconds);
}

void MinecraftWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);

    if (visualBlock) {
        g->clearTransform();
        g->setDefaultMaterial();
        g->translate(glm::vec3(position) + normal * 0.51f);

        if (normal.y == -1) {
            g->rotate(M_PI, glm::vec3(1, 0, 0));
        } else if (normal.y == 0) {
            glm::vec3 rt = glm::cross(normal, glm::vec3(0, 1, 0));
            g->rotate(-M_PI/2.f, rt);
        }

        g->scale(0.5f);
        g->drawShape("quad");
    }
}

void MinecraftWorld::reset() {

}

void MinecraftWorld::onMousePressed(QMouseEvent *event) {
    m_inputSystem->onMousePressed(event);
}

void MinecraftWorld::onMouseReleased(QMouseEvent *event) {
    m_inputSystem->onMouseReleased(event);
}

void MinecraftWorld::onMouseMoved(glm::vec2 &delta) {
    m_inputSystem->onMouseMoved(delta);
}

void MinecraftWorld::onMouseWheelMoved(QWheelEvent *event) {
    m_inputSystem->onMouseWheelMoved(event);
}
