#include "MenuWorld.h"

MenuWorld::MenuWorld() : elapsedTime(0), fixedTickTime(1.f/60.f) {
    m_timingSystem = std::make_shared<TimingSystem>();
    m_graphicsSystem = std::make_shared<GraphicsSystem>();
    m_inputSystem = std::make_shared<InputSystem>();

    addSystem(m_timingSystem);
    addSystem(m_graphicsSystem);
    addSystem(m_inputSystem);


    std::shared_ptr<GameObject> ob1 = std::make_shared<GameObject>("button");

    b1 = std::make_shared<MenuButton>(ob1.get(), "easy", "button", glm::vec2(0.35f, 0.6f), glm::vec2(0.3f, 0.2f));
    std::shared_ptr<TickComponent> tc = b1;
    std::shared_ptr<UIComponent> ui = b1;
    std::shared_ptr<InputComponent> in = b1;

    ob1->addComponent(tc);
    ob1->addComponent(ui);
    ob1->addComponent(in);

    std::shared_ptr<GameObject> ob2 = std::make_shared<GameObject>("button2");
    b2 = std::make_shared<MenuButton>(ob2.get(), "island", "button", glm::vec2(0.35f, 0.3f), glm::vec2(0.3f, 0.2f));
    tc = b2;
    ui = b2;
    in = b2;

    ob2->addComponent(tc);
    ob2->addComponent(ui);
    ob2->addComponent(in);

    //init();
    addGameObject("button", ob1);
    addGameObject("button2", ob2);
}

void MenuWorld::tick(float seconds) {
    elapsedTime += seconds;

    m_timingSystem->tick(seconds);

    while (elapsedTime >= fixedTickTime) {
        m_timingSystem->fixedTick(fixedTickTime);
        elapsedTime -= fixedTickTime;
    }

    m_timingSystem->lateTick(seconds);
}

void MenuWorld::draw(Graphics *g) {
    m_graphicsSystem->draw(g);
}

void MenuWorld::reset() {

}

void MenuWorld::onMousePressed(QMouseEvent *event) {
    m_inputSystem->onMousePressed(event);
}

void MenuWorld::onMouseReleased(QMouseEvent *event) {
    m_inputSystem->onMouseReleased(event);
}

void MenuWorld::onMouseMoved(glm::vec2 &delta) {
    m_inputSystem->onMouseMoved(delta);
}

void MenuWorld::onMouseWheelMoved(QWheelEvent *event) {
    m_inputSystem->onMouseWheelMoved(event);
}
