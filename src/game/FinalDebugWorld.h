#ifndef FINALDEBUGWORLD_H
#define FINALDEBUGWORLD_H


#include "engine/world/World.h"
#include "physics/volume/ConvexHull.h"
#include <random>
#include <chrono>

class RigidBody;

class FinalDebugWorld : public World
{
public:
    FinalDebugWorld(const std::vector<std::string> &shapes);

    void tick(float seconds);
    void draw(Graphics *g);
    void onKeyPressed(QKeyEvent *event);
    void onKeyReleased(QKeyEvent *event);
    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);


private:
    void makeTurn();

    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<PhysicsSystem> m_physicsSystem;

    unsigned int depthMap;
    unsigned int depthMapFBO;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

    unsigned int cubemapTexture;
    unsigned int skyboxVAO, skyboxVBO;

    std::vector<std::string> shapes;
    std::vector<std::shared_ptr<ConvexHull>> hulls;
    std::default_random_engine generator1, generator2;
    std::uniform_int_distribution<int> distribution;

    std::chrono::high_resolution_clock::time_point leftTime;
    std::shared_ptr<RigidBody> interaction;
    float interactionDist;
    bool dragging;
    bool start;
    int numObjects;
    float forceMult = 5.f;

    int player1score, player2score;
    std::chrono::high_resolution_clock::time_point p1timer, p2timer;
    const int numPlayers = 2;
    int playerTurn;
    const int moveCount = 3;
    int movesLeft;

    //std::shared_ptr<Transform> shapeTransform;
};

#endif // FINALDEBUGWORLD_H
