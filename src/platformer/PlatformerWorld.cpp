#include "PlatformerWorld.h"

#include <QCoreApplication>
#include <QApplication>

#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/physics/RigidBody.h"
#include "engine/graphics/Camera.h"
#include "PlatformerScreen.h"

#include "Player.h"


PlatformerWorld::PlatformerWorld() : elapsedTime(0), fixedTickTime(1.f/60.f) {
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();
    m_inputSystem = std::make_shared<InputSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);
    addSystem(m_inputSystem);

    init();
}


void PlatformerWorld::tick(float seconds) {
    elapsedTime += seconds;

    m_timingSystem->tick(seconds);

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);

        m_physicsSystem->update(fixedTickTime);
        elapsedTime -= fixedTickTime;
    }

    m_timingSystem->lateTick(seconds);
}

void PlatformerWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);
}

void PlatformerWorld::reset() {

}

void PlatformerWorld::onKeyPressed(QKeyEvent *event) {

}

void PlatformerWorld::onKeyReleased(QKeyEvent *event) {

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
