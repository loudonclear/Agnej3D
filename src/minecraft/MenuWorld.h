#ifndef MENUWORLD_H
#define MENUWORLD_H


#include "engine/world/World.h"
#include "engine/Application.h"
#include "minecraft/MenuButton.h"

class MenuWorld : public World
{
public:
    MenuWorld();

    void tick(float seconds);
    void draw(Graphics *g);
    void reset();

    void onMousePressed(QMouseEvent *event);
    void onMouseReleased(QMouseEvent *event);
    void onMouseMoved(glm::vec2 &delta);
    void onMouseWheelMoved(QWheelEvent *event);

    void setApplication(Application *app) {
        button->setApplication(app);
    }

private:
    float elapsedTime;
    float fixedTickTime;

    std::shared_ptr<TimingSystem> m_timingSystem;
    std::shared_ptr<GraphicsSystem> m_graphicsSystem;
    std::shared_ptr<InputSystem> m_inputSystem;

    std::shared_ptr<MenuButton> button;
};

#endif // MENUWORLD_H
