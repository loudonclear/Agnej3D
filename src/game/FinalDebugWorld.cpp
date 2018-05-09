#include "FinalDebugWorld.h"
#include "engine/input/Input.h"
#include "engine/graphics/Camera.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "graphics/Obj.h"
#include "physics/RigidBody.h"
#include "physics/ShapeCollider.h"
#include "physics/volume/ConvexCollider.h"
#include "physics/volume/BoxCollider.h"


FinalDebugWorld::FinalDebugWorld(const std::vector<std::string> &shapes) : elapsedTime(0), fixedTickTime(1.f/60.f), shapes(shapes), interaction(nullptr), dragging(false), numObjects(0)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);

    generator = std::default_random_engine(1);
    distribution = std::uniform_int_distribution<int>(0, shapes.size() - 1);
    std::uniform_real_distribution<float> floatDistribution = std::uniform_real_distribution<float>(0.f, 1.f);

    for (const std::string &shape : shapes) {
        std::shared_ptr<OBJ> obj = Graphics::getGlobalInstance()->getOBJ(shape);
        hulls.push_back(std::make_shared<ConvexHull>(obj->vertices.toStdVector(), obj->faces.toStdVector()));

        Material m;
        m.color = glm::vec3(floatDistribution(generator), floatDistribution(generator), floatDistribution(generator));
        Graphics::getGlobalInstance()->addMaterial(shape, m);
    }


    std::shared_ptr<GameObject> ground = std::make_shared<GameObject>("ground");
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(ground.get(), "oldcube", "grass");

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(ground.get());
    trans->setPosition(glm::vec3(-7.5f, 0, 0));
    trans->setScale(glm::vec3(10.f, 0.3f, 10.f));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<BoxCollider>(ground.get(), 0.f);
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(ground.get(), true);

    ground->addComponent(comp);
    ground->addComponent(trans);
    ground->addComponent(sc);
    ground->addComponent(rb);
    addGameObject("ground", ground);
    numObjects++;


    std::shared_ptr<GameObject> ground2 = std::make_shared<GameObject>("ground2");
    std::shared_ptr<GraphicsComponent> comp2 = std::make_shared<GraphicsShapeComponent>(ground2.get(), "oldcube", "grass");

    std::shared_ptr<Transform> trans2 = std::make_shared<Transform>(ground2.get());
    trans2->setPosition(glm::vec3(7.5f, 0, 0));
    trans2->setScale(glm::vec3(10.f, 0.3f, 10.f));

    std::shared_ptr<ShapeCollider> sc2 = std::make_shared<BoxCollider>(ground2.get(), 0.f);
    std::shared_ptr<RigidBody> rb2 = std::make_shared<RigidBody>(ground2.get(), true);

    ground2->addComponent(comp2);
    ground2->addComponent(trans2);
    ground2->addComponent(sc2);
    ground2->addComponent(rb2);
    addGameObject("ground2", ground2);
    numObjects++;


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


    if (interaction) {
        std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();
        std::shared_ptr<Transform> t = interaction->getTransform();

        if (!dragging) {
            t->setPosition(c->getEye() + c->getLook() * interactionDist);
        }

        const float rotSpeed = 1.f;
        if (Input::isKeyDown(Qt::Key_Q)) {
            t->rotateAngularAmt(glm::vec3(1, 0, 0) * rotSpeed * seconds);
        }
        if (Input::isKeyDown(Qt::Key_E)) {
            t->rotateAngularAmt(glm::vec3(0, 0, 1) * rotSpeed * seconds);
        }
        if (Input::isKeyDown(Qt::Key_R)) {
            t->setRotation(glm::quat(glm::vec3(0.f)));
        }
    }
}

void FinalDebugWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);
    std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();

    if (dragging && interaction) {
//        g->clearTransform();
//        g->setDefaultMaterial();
//        glm::vec3 pos = 0.5f * (interaction->getTransform()->getPosition() + c->getEye());
//        g->translate(pos);
//        g->scale(glm::vec3(0.5f*interactionDist, 0.05f, 0.05f));
//        g->rotate(90.f, glm::mat3x3(glm::lookAt(c->getEye(), interaction->getTransform()->getPosition(), c->getUp())) * glm::normalize(interaction->getTransform()->getPosition() - c->getEye()));

//        g->drawShape("oldcube");
    }
}

void FinalDebugWorld::onKeyPressed(QKeyEvent *event) {

}

void FinalDebugWorld::onKeyReleased(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        std::shared_ptr<GameObject> shape = std::make_shared<GameObject>("shape" + numObjects);

        int shapeNum = distribution(generator);
        std::string shapeName = shapes.at(shapeNum);
        std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(shape.get(), shapeName, shapeName);

        std::shared_ptr<Transform> trans = std::make_shared<Transform>(shape.get());
        trans->setPosition(glm::vec3(0.0f, 15.f, 0.5f));

        std::shared_ptr<ShapeCollider> sc = std::make_shared<ConvexCollider>(shape.get(), hulls.at(shapeNum), 10.f);
        std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(shape.get());

        shape->addComponent(trans);
        shape->addComponent(comp);
        shape->addComponent(sc);
        shape->addComponent(rb);

        numObjects++;
        shape->addToWorld(this);
        addGameObject("shape" + numObjects, shape);
    }
}

void FinalDebugWorld::onMousePressed(QMouseEvent *event) {

    if (event->button() == Qt::MouseButton::RightButton) {
        dragging = true;
    }
}

void FinalDebugWorld::onMouseReleased(QMouseEvent *event) {
    std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();

    if (event->button() == Qt::MouseButton::LeftButton) {
        if (interaction) {
            interaction->isStatic = false;
            interaction->interaction = false;
            interaction = nullptr;
        } else {
            Ray ray(c->getEye(), c->getLook());
            RaycastResult res;
            std::shared_ptr<RigidBody> hit = nullptr;
            if (m_physicsSystem->raycast(ray, res, hit) && res.t < 25.f && !hit->getCollider()->isStatic()) {
                interaction = hit;
                interaction->isStatic = true;
                interaction->interaction = true;
                interaction->awake = true;
                interaction->velocity = glm::vec3(0.f);
                interaction->angularVelocity = glm::vec3(0.f);
                interactionDist = glm::distance(interaction->getTransform()->getPosition(), c->getEye());
            }
        }
    } else if (event->button() == Qt::MouseButton::RightButton) {
        dragging = false;
        if (interaction) {

            Ray ray(c->getEye(), c->getLook());
            RaycastResult res;
            std::shared_ptr<RigidBody> hit = nullptr;

            if (m_physicsSystem->raycast(ray, res, hit)) {
                if (hit != interaction) {
                    glm::vec3 dir = glm::normalize(res.point - interaction->getTransform()->getPosition());
                    interaction->addForce(dir * forceMult);
                    interaction->isStatic = false;
                    interaction->interaction = false;
                    interaction = nullptr;
                }
            } else {
                glm::vec3 dir = glm::normalize(c->getEye() + c->getLook() * 25.f - interaction->getTransform()->getPosition());
                interaction->addForce(dir * forceMult);
                interaction->isStatic = false;
                interaction->interaction = false;
                interaction = nullptr;
            }
        }
    }

}

void FinalDebugWorld::onMouseMoved(glm::vec2 &delta) {

}

void FinalDebugWorld::onMouseWheelMoved(QWheelEvent *event) {

    if (interaction) {
        if (dragging) {
            const float forceMultSpeed = 0.005f;
            forceMult = glm::clamp(event->delta() * forceMultSpeed + forceMult, 5.f, 20.f);
        } else {
            float moveSpeed = 0.005f;
            interactionDist += moveSpeed * event->delta();
            interactionDist = glm::clamp(interactionDist, 2.f, 25.f);
        }
    }
}
