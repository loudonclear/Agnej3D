#ifndef FINALMENUSCREEN_H
#define FINALMENUSCREEN_H

#include "engine/Screen.h"

class Camera;
class Graphics;

class FinalMenuScreen : public Screen
{
public:
    FinalMenuScreen();

    void tick(float seconds);
    void draw(Graphics *g);

    void onKeyPressed(QKeyEvent *event);
    void resize(glm::vec2 &size);

private:
    std::shared_ptr<Camera> m_camera;
};

#endif // FINALMENUSCREEN_H
