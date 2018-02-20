#include "MinecraftWorld.h"

#include "MinecraftWorld.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/components/FixedTickComponent.h"
#include "engine/physics/RigidBody.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/graphics/Camera.h"
#include "minecraft/Chunk.h"
#include "minecraft/ChunkManager.h"
#include "minecraft/PerlinNoise.h"
#include "minecraft/Player.h"

MinecraftWorld::MinecraftWorld() : elapsedTime(0), fixedTickTime(1.f/60.f)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);
    m_chunkManager = std::make_shared<ChunkManager>(this, 32, 32, 32, 10, 10);


    addSystem(m_chunkManager);

    std::shared_ptr<GameObject> player = std::make_shared<Player>("player");
    addGameObject("player", player);


    init();
}


void MinecraftWorld::tick(float seconds) {
    elapsedTime += seconds;
    m_timingSystem->tick(seconds);
    m_chunkManager->update(seconds);

    while (elapsedTime >= fixedTickTime) {

        m_timingSystem->fixedTick(fixedTickTime);

        //m_physicsSystem->update(fixedTickTime);

        elapsedTime -= fixedTickTime;
    } 

    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    glm::vec3 look = c->getLook();
    c->translate(-look*zoom);
}

void MinecraftWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);
}

void MinecraftWorld::onMouseMoved(glm::vec2 &delta) {
    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    c->rotate(-delta.x / 100.f, -delta.y / 100.f);
}

void MinecraftWorld::onMouseWheelMoved(QWheelEvent *event) {

//    zoom += -event->delta() / 20.f;
//    zoom = glm::clamp(zoom, 0.5f, 3.f);
}
