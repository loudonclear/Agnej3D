#include "WarmupGameWorld.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/CylinderCollider.h"
#include "engine/physics/BoxCollider.h"
#include "engine/physics/SphereCollider.h"
#include "warmup/EnemyFollowComponent.h"
#include "warmup/components/GoalComponent.h"
#include "warmup/WarmupGameScreen.h"
#include "engine/graphics/Camera.h"

WarmupGameWorld::WarmupGameWorld() : elapsedTime(0), fixedTickTime(1.f/60.f)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<CollisionSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);

    std::shared_ptr<GameObject> player = std::make_shared<GameObject>("player");
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(player.get(), "cylinder");

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(player.get());
    playerTransform = trans;
    trans->setPosition(glm::vec3(-4, 0.5f, 0));
    std::shared_ptr<Camera> c = WarmupGameScreen::camera;
    c->setEye(glm::vec3(-4, 2.5f, 0));
    c->setLook(glm::vec3(1, 0, 0));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<CylinderCollider>(player.get());
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(player.get(), false);

    player->addComponent(comp);
    player->addComponent(trans);
    player->addComponent(sc);
    player->addComponent(rb);
    addGameObject("player", player);


    std::shared_ptr<GameObject> enemy = std::make_shared<GameObject>("enemy");
    comp = std::make_shared<GraphicsShapeComponent>(enemy.get(), "cylinder");

    trans = std::make_shared<Transform>(enemy.get());
    trans->setPosition(glm::vec3(4, 0.5f, 0));

    sc = std::make_shared<CylinderCollider>(enemy.get());
    rb = std::make_shared<RigidBody>(enemy.get(), false);

    std::shared_ptr<TickComponent> tc = std::make_shared<EnemyFollowComponent>(enemy.get());

    enemy->addComponent(comp);
    enemy->addComponent(trans);
    enemy->addComponent(sc);
    enemy->addComponent(rb);
    enemy->addComponent(tc);
    addGameObject("enemy", enemy);


    std::shared_ptr<GameObject> ball = std::make_shared<GameObject>("ball");
    comp = std::make_shared<GraphicsShapeComponent>(ball.get(), "sphere");

    trans = std::make_shared<Transform>(ball.get());
    trans->setPosition(glm::vec3(0, 0.5f, 0));

    sc = std::make_shared<SphereCollider>(ball.get());
    rb = std::make_shared<RigidBody>(ball.get(), false);

    ball->addComponent(comp);
    ball->addComponent(trans);
    ball->addComponent(sc);
    ball->addComponent(rb);
    addGameObject("ball", ball);


    std::shared_ptr<GameObject> wall1 = std::make_shared<GameObject>("wall");
    comp = std::make_shared<GraphicsShapeComponent>(wall1.get(), "cube");

    trans = std::make_shared<Transform>(wall1.get());
    trans->setPosition(glm::vec3(0, 0.5f, 2.5f));
    trans->setScale(glm::vec3(10, 1, 0.2f));

    sc = std::make_shared<BoxCollider>(wall1.get());

    wall1->addComponent(comp);
    wall1->addComponent(trans);
    wall1->addComponent(sc);
    addGameObject("wall1", wall1);


    std::shared_ptr<GameObject> wall2 = std::make_shared<GameObject>("wall");
    comp = std::make_shared<GraphicsShapeComponent>(wall2.get(), "cube");

    trans = std::make_shared<Transform>(wall2.get());
    trans->setPosition(glm::vec3(0, 0.5f, -2.5f));
    trans->setScale(glm::vec3(10, 1, 0.2f));

    sc = std::make_shared<BoxCollider>(wall2.get());

    wall2->addComponent(comp);
    wall2->addComponent(trans);
    wall2->addComponent(sc);
    addGameObject("wall2", wall2);


    std::shared_ptr<GameObject> goal1 = std::make_shared<GameObject>("goal");
    comp = std::make_shared<GraphicsShapeComponent>(goal1.get(), "cube");

    trans = std::make_shared<Transform>(goal1.get());
    trans->setPosition(glm::vec3(-5, 0.5f, 0));
    trans->setScale(glm::vec3(0.2f, 0.6f, 5));

    sc = std::make_shared<GoalComponent>(goal1.get(), score1);

    goal1->addComponent(comp);
    goal1->addComponent(trans);
    goal1->addComponent(sc);
    addGameObject("goal1", goal1);


    std::shared_ptr<GameObject> goal2 = std::make_shared<GameObject>("goal");
    comp = std::make_shared<GraphicsShapeComponent>(goal2.get(), "cube");

    trans = std::make_shared<Transform>(goal2.get());
    trans->setPosition(glm::vec3(5, 0.5f, 0));
    trans->setScale(glm::vec3(0.2f, 0.6f, 5));

    sc = std::make_shared<GoalComponent>(goal2.get(), score2);

    goal2->addComponent(comp);
    goal2->addComponent(trans);
    goal2->addComponent(sc);
    addGameObject("goal2", goal2);


    init();
}


void WarmupGameWorld::tick(float seconds) {
    elapsedTime += seconds;
    m_timingSystem->tick(seconds);

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);
        m_physicsSystem->update(fixedTickTime);

        elapsedTime -= fixedTickTime;
    }

    std::shared_ptr<Camera> c = WarmupGameScreen::camera;
    c->setEye(playerTransform->getPosition());
    glm::vec3 look = c->getLook();
    c->translate(-look*zoom);

    glm::vec3 dir = glm::normalize(glm::vec3(look.x, 0, look.z));
    glm::vec3 perp = glm::vec3(dir.z, 0, -dir.x);

    const float moveSpeed = 2.f;

    if (Input::isKeyDown(Qt::Key_W)) {
        c->translate(dir*seconds*moveSpeed);
        playerTransform->translate(dir*seconds*moveSpeed);
    }
    if (Input::isKeyDown(Qt::Key_S)) {
        c->translate(-dir*seconds*moveSpeed);
        playerTransform->translate(-dir*seconds*moveSpeed);
    }
    if (Input::isKeyDown(Qt::Key_A)) {
        c->translate(perp*seconds*moveSpeed);
        playerTransform->translate(perp*seconds*moveSpeed);
    }
    if (Input::isKeyDown(Qt::Key_D)) {
        c->translate(-perp*seconds*moveSpeed);
        playerTransform->translate(-perp*seconds*moveSpeed);
    }
}

void WarmupGameWorld::draw(Graphics *g) {
    std::shared_ptr<Camera> c = WarmupGameScreen::camera;

    c->setUI(true);

    g->clearTransform();
    g->translate(glm::vec3(6.f, 2, -0.5f));
    g->rotate(glm::radians(-90.f), glm::vec3(0, 1, 0));
    g->drawText("default", std::to_string(score1), 2);

    g->clearTransform();
    g->translate(glm::vec3(-6.f, 2, 0.5f));
    g->rotate(glm::radians(90.f), glm::vec3(0, 1, 0));
    g->drawText("default", std::to_string(score2), 2);

    c->setUI(false);

    g->clearTransform();
    g->scale(glm::vec3(10, 1, 5));
    g->setMaterial("grass");
    g->drawShape("quad");

    m_graphicsSystem->draw(g);
}

void WarmupGameWorld::onMouseMoved(glm::vec2 &delta) {
    std::shared_ptr<Camera> c = WarmupGameScreen::camera;
    c->rotate(-delta.x / 100.f, -delta.y / 100.f);
}

void WarmupGameWorld::onMouseWheelMoved(QWheelEvent *event) {

    zoom += -event->delta() / 20.f;
    zoom = glm::clamp(zoom, 0.5f, 3.f);
}
