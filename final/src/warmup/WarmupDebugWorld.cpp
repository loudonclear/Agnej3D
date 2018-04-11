#include "WarmupDebugWorld.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "engine/components/FixedTickComponent.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/CylinderCollider.h"
#include "engine/physics/BoxCollider.h"
#include "engine/physics/SphereCollider.h"
#include "engine/physics/ConvexTriangleMeshCollider.h"
#include "warmup/components/VisualCollide.h"

WarmupDebugWorld::WarmupDebugWorld() : elapsedTime(0), fixedTickTime(1.f/60.f)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);

    Graphics *graphics = Graphics::getGlobalInstance();
    Material notColliding;
    notColliding.color = glm::vec3(0, 1, 0);
    graphics->addMaterial("notColliding", notColliding);

    Material colliding;
    colliding.color = glm::vec3(1, 0, 0);
    graphics->addMaterial("colliding", colliding);


    interaction = std::make_shared<GameObject>("interaction");
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(interaction, "tetrahedron");

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(interaction);
    trans->setPosition(glm::vec3(0, 0.5f, -4.f));
    shapeTransform = trans;

    std::vector<glm::vec3> verts;
    verts.push_back(glm::vec3(0, 0.82f, 0));
    verts.push_back(glm::vec3(0.5f, 0, -0.3f));
    verts.push_back(glm::vec3(-0.5f, 0, -0.3f));
    verts.push_back(glm::vec3(0, 0, 0.6));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<ConvexTriangleMeshCollider>(interaction, verts);
    std::shared_ptr<TickComponent> vc = std::make_shared<VisualCollide>(interaction);

    interaction->addComponent(comp);
    interaction->addComponent(trans);
    interaction->addComponent(sc);
    interaction->addComponent(vc);
    addGameObject("interaction", interaction);


    std::shared_ptr<GameObject> cylinder = std::make_shared<GameObject>("cylinder");
    std::shared_ptr<GraphicsComponent> comp1 = std::make_shared<GraphicsShapeComponent>(cylinder, "cylinder");

    std::shared_ptr<Transform> trans1 = std::make_shared<Transform>(cylinder);
    trans1->setPosition(glm::vec3(-4, 0.5f, 0));

    std::shared_ptr<ShapeCollider> sc1 = std::make_shared<CylinderCollider>(cylinder);
    vc = std::make_shared<VisualCollide>(cylinder);

    cylinder->addComponent(comp1);
    cylinder->addComponent(trans1);
    cylinder->addComponent(sc1);
    cylinder->addComponent(vc);
    addGameObject("cylinder", cylinder);


    std::shared_ptr<GameObject> cube = std::make_shared<GameObject>("cube");
    std::shared_ptr<GraphicsComponent> comp2 = std::make_shared<GraphicsShapeComponent>(cube, "cube");

    std::shared_ptr<Transform> trans2 = std::make_shared<Transform>(cube);
    trans2->setPosition(glm::vec3(0, 0.5f, 0));

    std::shared_ptr<ShapeCollider> sc2 = std::make_shared<BoxCollider>(cube);
    vc = std::make_shared<VisualCollide>(cube);

    cube->addComponent(comp2);
    cube->addComponent(trans2);
    cube->addComponent(sc2);
    cube->addComponent(vc);
    addGameObject("cube", cube);


    std::shared_ptr<GameObject> ctm = std::make_shared<GameObject>("ctm");
    std::shared_ptr<GraphicsComponent> comp3 = std::make_shared<GraphicsShapeComponent>(ctm, "tetrahedron");

    std::shared_ptr<Transform> trans3 = std::make_shared<Transform>(ctm);
    trans3->setPosition(glm::vec3(4, 0.5f, 0));


    std::shared_ptr<ShapeCollider> sc3 = std::make_shared<ConvexTriangleMeshCollider>(ctm, verts);
    vc = std::make_shared<VisualCollide>(ctm);

    ctm->addComponent(comp3);
    ctm->addComponent(trans3);
    ctm->addComponent(sc3);
    ctm->addComponent(vc);
    addGameObject("ctm", ctm);

    init();
}


void WarmupDebugWorld::tick(float seconds) {
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

void WarmupDebugWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);
}

void WarmupDebugWorld::onKeyPressed(QKeyEvent *event) {

    if (event->key() == Qt::Key_Space) {
        removeGameObject(interaction->name);

        interaction = std::make_shared<GameObject>("interaction");
        std::shared_ptr<GraphicsComponent> comp;
        std::shared_ptr<ShapeCollider> sc;

        shapeType = (shapeType + 1) % 3;
        switch(shapeType) {
        case 0:
            comp = std::make_shared<GraphicsShapeComponent>(interaction, "cylinder");
            sc = std::make_shared<CylinderCollider>(interaction);
            break;
        case 1:
            comp = std::make_shared<GraphicsShapeComponent>(interaction, "cube");
            sc = std::make_shared<BoxCollider>(interaction);
            break;
        case 2:
            std::vector<glm::vec3> verts;
            verts.push_back(glm::vec3(0, 0.82f, 0));
            verts.push_back(glm::vec3(0.5f, 0, -0.3f));
            verts.push_back(glm::vec3(-0.5f, 0, -0.3f));
            verts.push_back(glm::vec3(0, 0, 0.6));

            comp = std::make_shared<GraphicsShapeComponent>(interaction, "tetrahedron");
            sc = std::make_shared<ConvexTriangleMeshCollider>(interaction, verts);
            break;
        }

        std::shared_ptr<Transform> trans = std::make_shared<Transform>(interaction);
        trans->setPosition(glm::vec3(0, 0.5f, -4.f));
        shapeTransform = trans;


        std::shared_ptr<TickComponent> vc = std::make_shared<VisualCollide>(interaction);

        interaction->addComponent(comp);
        interaction->addComponent(trans);
        interaction->addComponent(sc);
        interaction->addComponent(vc);
        addGameObject("interaction", interaction);
        interaction->addToWorld(this);
    }

}
