#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine/util/CommonIncludes.h"
#include <QKeyEvent>
#include <memory>

class Graphics;
class Screen;

class Application
{
public:
    Application();

    void addScreen(const std::string &name, const std::shared_ptr<Screen> &screen);
    void removeScreen(const std::string &name);
    void setScreen(const std::string &name);
    std::shared_ptr<Screen> getScreen(const std::string &name);
    std::shared_ptr<Screen> getActiveScreen();

    bool captureMouse = true;

    virtual void tick(float seconds);
    virtual void draw(Graphics *g);
    virtual void resize(glm::vec2 size);

    virtual void onKeyPressed(QKeyEvent *event);
    virtual void onKeyReleased(QKeyEvent *event);
    virtual void onMousePressed(QMouseEvent *event);
    virtual void onMouseReleased(QMouseEvent *event);
    virtual void onMouseMoved(glm::vec2 delta);
    virtual void onMouseWheelMoved(QWheelEvent *event);

private:
    std::shared_ptr<Screen> m_activeScreen;
    std::map<std::string, std::shared_ptr<Screen>> m_screens;

};

#endif // APPLICATION_H
