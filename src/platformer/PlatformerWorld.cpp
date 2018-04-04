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


PlatformerWorld::PlatformerWorld(std::string level, std::string levelNav) : elapsedTime(0), fixedTickTime(1.f/60.f), m_level(level), m_levelNav(levelNav), showNavMesh(false), start(nullptr), end(nullptr) {
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_collisionSystem = std::make_shared<CollisionSystem>();
    m_inputSystem = std::make_shared<InputSystem>();
    m_objCollisionSystem = std::make_shared<OBJCollisionSystem>(Graphics::getGlobalInstance()->getOBJ(level));

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_collisionSystem);
    addSystem(m_inputSystem);

    m_navMesh = std::make_shared<NavMesh>(Graphics::getGlobalInstance()->getOBJ(levelNav));


    std::shared_ptr<GameObject> player = std::make_shared<Player>("player", m_objCollisionSystem);
    pt = player->getComponent<Transform>();
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

    start = m_navMesh->getTriangle(Ray(pt->getPosition(), glm::vec3(0, -1, 0)), sr);

    if (start != nullptr && end != nullptr) {
        m_navMesh->getPath(start, end);
    }
}

void PlatformerWorld::draw(Graphics *g) {
    g->setCamera(PlatformerScreen::camera);
    m_graphicsSystem->draw(g);

    g->clearTransform();
    g->setMaterial(m_level);
    g->drawShape(m_level);

    if (showNavMesh) {
        g->clearTransform();
        g->translate(glm::vec3(0, 0.001f, 0));
        g->setMaterial("nav");
        g->drawShape(m_levelNav);


        if (start != nullptr) {
            g->clearTransform();
            g->translate(sr.point);
            g->scale(glm::vec3(0.8f, 1.8f, 0.8f));
            g->drawShape("sphere");
        }
        if (end != nullptr) {
            g->clearTransform();
            g->translate(er.point);
            g->scale(glm::vec3(0.8f, 1.8f, 0.8f));
            g->drawShape("sphere");
        }

        if (start != nullptr && end != nullptr) {
            m_navMesh->drawPath(g);
        }
    }
}

void PlatformerWorld::reset() {

}

void PlatformerWorld::onKeyPressed(QKeyEvent *event) {
    if (event->key() == Qt::Key_P) {
        showNavMesh = !showNavMesh;
    }
}

void PlatformerWorld::onKeyReleased(QKeyEvent *event) {

}

void PlatformerWorld::onMousePressed(QMouseEvent *event) {
    m_inputSystem->onMousePressed(event);

    if (event->button() == Qt::RightButton) {
        std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();
        end = m_navMesh->getTriangle(Ray(c->getEye(), c->getLook()), er);
    }
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
