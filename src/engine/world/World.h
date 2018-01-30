#ifndef WORLD_H
#define WORLD_H

#include <QKeyEvent>
#include <glm/glm.hpp>

class Graphics;

class World
{
public:
    World();

   // bool addSystem();
   // bool removeSystem();

    bool addGameObject();
    bool removeGameObject();

    virtual void tick(float seconds) {}
    virtual void draw(Graphics *g) {}

    virtual void onMousePressed(QMouseEvent *event) {}
    virtual void onMouseMoved(glm::vec2 delta) {}
    virtual void onMouseReleased(QMouseEvent *event) {}
    virtual void onMouseWheelMoved(QWheelEvent *event) {}
    virtual void onKeyPressed(QKeyEvent *event) {}
    virtual void onKeyReleased(QKeyEvent *event) {}

};

#endif // WORLD_H
