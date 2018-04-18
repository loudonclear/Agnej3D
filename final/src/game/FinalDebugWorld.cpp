#include "FinalDebugWorld.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/BoxCollider.h"
#include "engine/physics/ConvexTriangleMeshCollider.h"
#include "engine/physics/RigidBody.h"

FinalDebugWorld::FinalDebugWorld() : elapsedTime(0), fixedTickTime(1.f/60.f)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);

    interaction = std::make_shared<GameObject>("interaction");
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(interaction.get(), "tetrahedron");

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(interaction.get());
    trans->setPosition(glm::vec3(0, 0.5f, -4.f));
    shapeTransform = trans;

    std::vector<glm::vec3> verts;
    verts.push_back(glm::vec3(0, 0.82f, 0));
    verts.push_back(glm::vec3(0.5f, 0, -0.3f));
    verts.push_back(glm::vec3(-0.5f, 0, -0.3f));
    verts.push_back(glm::vec3(0, 0, 0.6));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<ConvexTriangleMeshCollider>(interaction.get(), verts);
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(interaction.get());

    interaction->addComponent(comp);
    interaction->addComponent(trans);
    interaction->addComponent(sc);
    interaction->addComponent(rb);
    addGameObject("interaction", interaction);


    std::shared_ptr<GameObject> cube = std::make_shared<GameObject>("cube");
    std::shared_ptr<GraphicsComponent> comp2 = std::make_shared<GraphicsShapeComponent>(cube.get(), "cube");

    std::shared_ptr<Transform> trans2 = std::make_shared<Transform>(cube.get());
    trans2->setPosition(glm::vec3(0, 0.5f, 0));

    std::shared_ptr<ShapeCollider> sc2 = std::make_shared<BoxCollider>(cube.get());
    rb = std::make_shared<RigidBody>(cube.get());

    cube->addComponent(comp2);
    cube->addComponent(trans2);
    cube->addComponent(sc2);
    cube->addComponent(rb);
    addGameObject("cube", cube);


    std::shared_ptr<GameObject> ctm = std::make_shared<GameObject>("ctm");
    std::shared_ptr<GraphicsComponent> comp3 = std::make_shared<GraphicsShapeComponent>(ctm.get(), "cube", "grass");

    std::shared_ptr<Transform> trans3 = std::make_shared<Transform>(ctm.get());
    trans3->setPosition(glm::vec3(0, -1.0f, 0));
    trans3->setScale(glm::vec3(20.f, 0.3f, 20.f));


    std::shared_ptr<ShapeCollider> sc3 = std::make_shared<BoxCollider>(ctm.get());
    rb = std::make_shared<RigidBody>(ctm.get(), true);

    ctm->addComponent(comp3);
    ctm->addComponent(trans3);
    ctm->addComponent(sc3);
    ctm->addComponent(rb);
    addGameObject("ctm", ctm);

    init();
}


void FinalDebugWorld::tick(float seconds) {
    elapsedTime += seconds;
    m_timingSystem->tick(seconds);

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);
        m_physicsSystem->update(fixedTickTime);

        elapsedTime -= fixedTickTime;
    }

    if (Input::isKeyDown(Qt::Key_Left)) {
        shapeTransform->translate(glm::vec3(seconds, 0, 0));
    }
    if (Input::isKeyDown(Qt::Key_Right)) {
        shapeTransform->translate(glm::vec3(-seconds, 0, 0));
    }
    if (Input::isKeyDown(Qt::Key_Up)) {
        shapeTransform->translate(glm::vec3(0, 0, seconds));
    }
    if (Input::isKeyDown(Qt::Key_Down)) {
        shapeTransform->translate(glm::vec3(0, 0, -seconds));
    }
    if (Input::isKeyDown(Qt::Key_N)) {
        shapeTransform->translate(glm::vec3(0, seconds, 0));
    }
    if (Input::isKeyDown(Qt::Key_M)) {
        shapeTransform->translate(glm::vec3(0, -seconds, 0));
    }


    if (Input::isKeyDown(Qt::Key_Q)) {
        shapeTransform->scale(glm::vec3(-seconds, -seconds, -seconds));
    }
    if (Input::isKeyDown(Qt::Key_E)) {
        shapeTransform->scale(glm::vec3(seconds, seconds, seconds));
    }
    if (Input::isKeyDown(Qt::Key_R)) {
        shapeTransform->rotate(glm::quat(glm::vec3(seconds, seconds, seconds)));
    }


    shapeTransform->setScale(glm::clamp(shapeTransform->getScale(), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(3, 3, 3)));

}

void FinalDebugWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);
}

void FinalDebugWorld::onKeyPressed(QKeyEvent *event) {

}
