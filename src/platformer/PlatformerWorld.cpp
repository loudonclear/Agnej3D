#include "PlatformerWorld.h"

#include <QCoreApplication>
#include <QApplication>

#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/physics/RigidBody.h"
#include "engine/graphics/Camera.h"
#include "engine/systems/OBJCollisionSystem.h"
#include "PlatformerScreen.h"

#include "Player.h"


PlatformerWorld::PlatformerWorld(std::string level) : elapsedTime(0), fixedTickTime(1.f/60.f), m_level(level) {
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_collisionSystem = std::make_shared<CollisionSystem>();
    m_inputSystem = std::make_shared<InputSystem>();
    m_objCollisionSystem = std::make_shared<OBJCollisionSystem>(Graphics::getGlobalInstance()->getOBJ(level));

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_collisionSystem);
    addSystem(m_inputSystem);


    std::shared_ptr<GameObject> player = std::make_shared<Player>("player", m_objCollisionSystem);
    addGameObject("player", player, false);

    init();
}


void PlatformerWorld::tick(float seconds) {
    elapsedTime += seconds;

    m_timingSystem->tick(seconds);

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);

        m_collisionSystem->update(fixedTickTime);
        elapsedTime -= fixedTickTime;
    }

    m_timingSystem->lateTick(seconds);
}

void PlatformerWorld::draw(Graphics *g) {
    g->setCamera(PlatformerScreen::camera);
    m_graphicsSystem->draw(g);

    g->clearTransform();
    g->setMaterial(m_level);
    g->drawShape(m_level);
}

void PlatformerWorld::reset() {

}

void PlatformerWorld::onMousePressed(QMouseEvent *event) {
    m_inputSystem->onMousePressed(event);
}

void PlatformerWorld::onMouseReleased(QMouseEvent *event) {
    m_inputSystem->onMouseReleased(event);
}

void PlatformerWorld::onMouseMoved(glm::vec2 &delta) {
    m_inputSystem->onMouseMoved(delta);
}

void PlatformerWorld::onMouseWheelMoved(QWheelEvent *event) {
    m_inputSystem->onMouseWheelMoved(event);
}
