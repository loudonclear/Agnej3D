#include "FinalDebugWorld.h"
#include "engine/input/Input.h"
#include "engine/graphics/Camera.h"
#include "engine/components/Transform.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/components/TickComponent.h"
#include "graphics/Obj.h"
#include "graphics/FBO.h"
#include "graphics/Font.h"
#include "graphics/Light.h"
#include "physics/ContactGenerator.h"
#include "physics/RigidBody.h"
#include "physics/ShapeCollider.h"
#include "physics/volume/ConvexCollider.h"
#include "physics/volume/BoxCollider.h"

#include <QImage>
#include <QString>
#include <QGLWidget>

unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < faces.size(); i++) {
        QImage image(QString::fromStdString(faces[i].c_str()));
        image = image.mirrored();
        image = QGLWidget::convertToGLFormat(image);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

FinalDebugWorld::FinalDebugWorld(const std::vector<std::string> &shapes) : elapsedTime(0), fixedTickTime(1.f/60.f), shapes(shapes), interaction(nullptr), dragging(false), numObjects(0)
{
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_physicsSystem = std::make_shared<PhysicsSystem>();

    playerTurn = 0;
    movesLeft = moveCount;
    start = false;

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_physicsSystem);

    Graphics::getGlobalInstance()->addFramebuffer("displayBuffer", std::make_shared<FBO>(800, 600));
    Graphics::getGlobalInstance()->addShader("fxaa", ":/shaders/quad.vert", ":/shaders/fxaa.frag");
    Graphics::getGlobalInstance()->addShader("skybox", ":/shaders/skybox.vert", ":/shaders/skybox.frag");
    Graphics::getGlobalInstance()->addShader("line", ":/shaders/line.vert", ":/shaders/line.frag");
    Graphics::getGlobalInstance()->addShader("shadow", ":/shaders/shadow.vert", ":/shaders/shadow.frag");


    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    std::vector<std::string> faces
    {
        ":/images/right.png",
        ":/images/left.png",
        ":/images/top.png",
        ":/images/bottom.png",
        ":/images/front.png",
        ":/images/back.png"
    };
    cubemapTexture = loadCubemap(faces);

    float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


    generator1 = std::default_random_engine(1);
    distribution = std::uniform_int_distribution<int>(0, shapes.size() - 1);
    std::uniform_real_distribution<float> floatDistribution = std::uniform_real_distribution<float>(0.f, 1.f);

    for (const std::string &shape : shapes) {
        std::shared_ptr<OBJ> obj = Graphics::getGlobalInstance()->getOBJ(shape);
        hulls.push_back(std::make_shared<ConvexHull>(obj->vertices.toStdVector(), obj->faces.toStdVector()));

        Material m;
        m.color = glm::vec3(floatDistribution(generator1), floatDistribution(generator1), floatDistribution(generator1));
        Graphics::getGlobalInstance()->addMaterial(shape, m);
    }

    generator1 = std::default_random_engine(1);
    generator2 = std::default_random_engine(1);

    std::shared_ptr<GameObject> ground = std::make_shared<GameObject>("ground");
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(ground.get(), "oldcube", "grass");

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(ground.get());
    trans->setPosition(glm::vec3(-8.5f, 0, 0));
    trans->setScale(glm::vec3(7.5f, 0.3f, 7.5f));

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
    trans2->setPosition(glm::vec3(8.5f, 0, 0));
    trans2->setScale(glm::vec3(7.5f, 0.3f, 7.5f));

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

    float tempplayer1Score = 0;
    float tempplayer2Score = 0;

    for (auto rb : m_physicsSystem->getComponents()) {
        if (rb->getCollider()->isStatic() || rb->interaction) continue;

        if (rb->getTransform()->getPosition().y < -500.f) {
            this->removeGameObject(rb->getGameObject()->name);
        }

        if (glm::length(rb->velocity) < 0.25f && glm::length(rb->angularVelocity) < 0.25f) {
            if (rb->getTransform()->getPosition().x > 0.f) {
                tempplayer1Score = glm::max(tempplayer1Score, 1.5f*rb->getTransform()->getPosition().y);
            } else {
                tempplayer2Score = glm::max(tempplayer2Score, 1.5f*rb->getTransform()->getPosition().y);
            }
        }
    }

    if (glm::round(tempplayer1Score) != player1score) {
        std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
        float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current - p1timer).count() / 1000.0;
        if (seconds >= 0.2f) {
            player1score = glm::round(tempplayer1Score);
        }
    } else {
        p1timer = std::chrono::high_resolution_clock::now();
    }
    if (glm::round(tempplayer2Score) != player2score) {
        std::chrono::high_resolution_clock::time_point current = std::chrono::high_resolution_clock::now();
        float seconds = std::chrono::duration_cast<std::chrono::milliseconds>(current - p2timer).count() / 1000.0;
        if (seconds >= 0.2f) {
            player2score = glm::round(tempplayer2Score);
        }
    } else {
        p2timer = std::chrono::high_resolution_clock::now();
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
            t->rotateAngularAmt(glm::vec3(0, 1, 0) * rotSpeed * seconds);
        }
        if (Input::isKeyDown(Qt::Key_R)) {
            t->setRotation(glm::quat(glm::vec3(0.f)));
        }
    }
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void FinalDebugWorld::draw(Graphics *g) {
    std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4x4 lightSpaceMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, 20.f) *
            glm::lookAt(-25.f * glm::vec3(-0.1f, -0.5f, 1), glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));

    g->setShader("shadow");
    g->getShader("shadow")->setUniform("lightSpaceMatrix", lightSpaceMatrix);
    m_graphicsSystem->draw(g);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    g->setFramebuffer("displayBuffer");
    g->clearScreen();

    // Skybox
    glDepthMask(GL_FALSE);
    g->setShader("skybox");
    g->getShader("skybox")->setUniform("skybox", 0);
    g->getShader("skybox")->setUniform("p", c->getProjection());
    g->getShader("skybox")->setUniform("v", glm::mat4x4(glm::mat3x3(c->getView())));
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    if (interaction) {
        if (playerTurn == 0 && interaction->getTransform()->getPosition().x < -0.5f) {
            interaction->getGameObject()->getComponent<GraphicsComponent>()->alpha = 0.20f;
        } else if (playerTurn == 1 && interaction->getTransform()->getPosition().x > 0.5f) {
            interaction->getGameObject()->getComponent<GraphicsComponent>()->alpha = 0.20f;
        } else {
            interaction->getGameObject()->getComponent<GraphicsComponent>()->alpha = 0.70;
        }
    }

    g->setDefaultShader();
    g->getActiveShader()->setUniform("lightSpaceMatrix", lightSpaceMatrix);

    g->getActiveShader()->setUniform("shadowMap", 5);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    m_graphicsSystem->draw(g);


    if (dragging && interaction) {
        glClear(GL_DEPTH_BUFFER_BIT);
        g->setShader("line");
        g->getShader("line")->setUniform("p", c->getProjection());
        g->getShader("line")->setUniform("v", c->getView());

        glm::vec3 p1 = c->getEye() + c->getLook() * 25.f;

        std::vector<float> lineverts;
        lineverts.push_back(p1.x); lineverts.push_back(p1.y); lineverts.push_back(p1.z);


        // line VAO
        unsigned int lineVAO, lineVBO;
        glGenVertexArrays(1, &lineVAO);
        glGenBuffers(1, &lineVBO);
        glBindVertexArray(lineVAO);
        glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lineverts),  lineverts.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindVertexArray(lineVAO);

        glPointSize(forceMult);
        glDrawArrays(GL_POINTS, 0, 1);
    }

    g->setDefaultFramebuffer();


    g->setShader("fxaa");
    g->getShader("fxaa")->setTexture("color", g->getFramebuffer("displayBuffer")->getColorAttachment(0));
    renderQuad();


    g->setDefaultShader();


    glm::vec2 screenSize = c->getScreenSize();
    c->setScreenSize(glm::vec2(1));
    c->setUI(true);
    g->setCamera(c);
    g->setDefaultMaterial();

    int p1s, p2s;
    if (glm::dot(c->getLook(), glm::vec3(0, 0, -1)) < 0.f) {
        p1s = player1score;
        p2s = player2score;
    } else {
        p2s = player1score;
        p1s = player2score;
    }

    c->setUI(true);
    g->clearTransform();
    g->translate(glm::vec3(0.05f, 0.90f, 0.f));
    g->drawText("dragon", std::to_string(p1s), 0.1f);

    g->clearTransform();
    g->translate(glm::vec3(0.88f, 0.90f, 0));
    g->drawText("dragon", std::to_string(p2s), 0.1f);
    c->setUI(false);

    g->clearTransform();
    g->translate(glm::vec3(0.44f, 0.95f, 0));
    g->drawText("dragon", "Player: " + std::to_string(playerTurn + 1), 0.05f);

    g->clearTransform();
    g->translate(glm::vec3(0.39f, 0.90f, 0));
    g->drawText("dragon", "Moves Left: " + std::to_string(movesLeft), 0.05f);

    c->setUI(false);
    c->setScreenSize(screenSize);
    g->setCamera(c);

}

void FinalDebugWorld::onKeyPressed(QKeyEvent *event) {

}

bool bint = false;
void FinalDebugWorld::makeTurn() {
    if (bint) {
        bint = false;
        return;
    }
    if (--movesLeft <= 0) {
        playerTurn = (playerTurn + 1) % numPlayers;
        movesLeft = moveCount;
    }
}

void FinalDebugWorld::onKeyReleased(QKeyEvent *event) {
    if (start && event->key() == Qt::Key_Space) {
        if (interaction) return;

        std::string name = "shape" + std::to_string(numObjects);
        std::shared_ptr<GameObject> shape = std::make_shared<GameObject>(name);

        int shapeNum = playerTurn == 0 ? distribution(generator1) : distribution(generator2);
        std::string shapeName = shapes.at(shapeNum);
        std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(shape.get(), shapeName, shapeName);

        std::shared_ptr<Transform> trans = std::make_shared<Transform>(shape.get());
        //trans->setPosition(glm::vec3(0.0f, 15.f, 0.5f));

        std::shared_ptr<ShapeCollider> sc = std::make_shared<ConvexCollider>(shape.get(), hulls.at(shapeNum), 10.f);
        std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(shape.get());

        shape->addComponent(trans);
        shape->addComponent(comp);
        shape->addComponent(sc);
        shape->addComponent(rb);

        numObjects++;
        shape->addToWorld(this);
        addGameObject(name, shape);

        interaction = rb;
        interaction->isStatic = true;
        interaction->interaction = true;
        interaction->awake = true;
        interaction->velocity = glm::vec3(0.f);
        interaction->angularVelocity = glm::vec3(0.f);
        interactionDist = 5.f;
        bint = true;
        makeTurn();
    } else {
        start = true;
    }
}

void FinalDebugWorld::onMousePressed(QMouseEvent *event) {
    std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();

    if (event->button() == Qt::MouseButton::RightButton) {
        dragging = true;
    } else if (event->button() == Qt::MouseButton::LeftButton) {
        if (interaction) {
            if (playerTurn == 0 && interaction->getTransform()->getPosition().x < -0.5f) return;
            if (playerTurn == 1 && interaction->getTransform()->getPosition().x > 0.5f) return;

            interaction->getGameObject()->getComponent<GraphicsComponent>()->alpha = 1.f;
            interaction->isStatic = false;
            interaction->interaction = false;
            interaction = nullptr;
            makeTurn();
        } else {
            Ray ray(c->getEye(), c->getLook());
            RaycastResult res;
            std::shared_ptr<RigidBody> hit = nullptr;
            if (m_physicsSystem->raycast(ray, res, hit) && !hit->getCollider()->isStatic()) {
                if (playerTurn == 0 && res.point.x < -0.5f) return;
                if (playerTurn == 1 && res.point.x > 0.5f) return;

                interaction = hit;
                interaction->isStatic = true;
                interaction->interaction = true;
                interaction->awake = true;
                interaction->velocity = glm::vec3(0.f);
                interaction->angularVelocity = glm::vec3(0.f);
                interactionDist = glm::distance(interaction->getTransform()->getPosition(), c->getEye());
            }
        }

        leftTime = std::chrono::high_resolution_clock::now();
    }
}

void FinalDebugWorld::onMouseReleased(QMouseEvent *event) {
    std::shared_ptr<Camera> c = Graphics::getGlobalInstance()->getActiveCamera();

     if (event->button() == Qt::MouseButton::RightButton) {
        dragging = false;
        if (interaction) {
            if (playerTurn == 0 && interaction->getTransform()->getPosition().x < -0.5f) return;
            if (playerTurn == 1 && interaction->getTransform()->getPosition().x > 0.5f) return;

            Ray ray(c->getEye(), c->getLook());
            RaycastResult res;
            std::shared_ptr<RigidBody> hit = nullptr;

            glm::vec3 dir;
            if (m_physicsSystem->raycast(ray, res, hit) && hit != interaction) {
                dir = glm::normalize(res.point - interaction->getTransform()->getPosition());
            } else {
                dir = glm::normalize(c->getEye() + c->getLook() * 25.f - interaction->getTransform()->getPosition());
            }

            interaction->addForce(dir * forceMult);
            interaction->getGameObject()->getComponent<GraphicsComponent>()->alpha = 1.f;
            interaction->isStatic = false;
            interaction->interaction = false;
            interaction = nullptr;
            makeTurn();
        }
     } else if (Qt::MouseButton::MiddleButton) {
         if (interaction && movesLeft >= 3) {

             if (playerTurn == 0 && interaction->getTransform()->getPosition().x < -0.5f) return;
             if (playerTurn == 1 && interaction->getTransform()->getPosition().x > 0.5f) return;

             ContactGenerator::Simplex simplex;
             ContactBasicData data;
             int count = 0;
             data.colliders[0] = interaction->getCollider();

             for (auto rb : m_physicsSystem->getComponents()) {
                 if (rb == interaction || !rb->getCollider()->isStatic()) continue;

                 data.colliders[1] = rb->getCollider();
                 if (ContactGenerator::GJK(&simplex, &data, &count)) {
                     interaction->getGameObject()->getComponent<GraphicsComponent>()->alpha = 1.f;
                     interaction->isStatic = true;
                     interaction->interaction = false;
                     interaction = nullptr;
                     makeTurn();
                     makeTurn();
                     makeTurn();
                     break;
                 }
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
            forceMult = glm::clamp(event->delta() * forceMultSpeed + forceMult, 5.f, 15.f);
        } else {
            float moveSpeed = 0.005f;
            interactionDist += moveSpeed * event->delta();
            interactionDist = glm::clamp(interactionDist, 2.f, 25.f);
        }
    }
}
