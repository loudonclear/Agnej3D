#ifndef WORLD_H
#define WORLD_H

#include <QKeyEvent>
#include <glm/glm.hpp>
#include "GameObject.h"
#include "engine/systems/System.h"
#include "engine/systems/TimingSystem.h"
#include "engine/systems/GraphicsSystem.h"
#include "engine/systems/PhysicsSystem.h"


class Graphics;

class World
{
public:
    World();

    template <typename SystemType>
    std::shared_ptr<SystemType> getSystem() {
        auto it = m_systems.find<SystemType>();
        if (it == m_systems.end()) return nullptr;
        return std::static_pointer_cast<SystemType>(it->second);
    }

    template <typename SystemType>
    void addSystem(std::shared_ptr<SystemType> system) {
        m_systems.put<SystemType>(system);
    }

    template <typename SystemType>
    void removeSystem() {
        m_systems.remove<SystemType>();
    }

    void addGameObject(const std::string &name, const std::shared_ptr<GameObject> &gameObject);
    void addGameObject(const std::string &name, GameObject &&gameObject);
    std::shared_ptr<GameObject> getGameObject(const std::string &name);
    void removeGameObject(const std::string &name);

    void init();

    virtual void tick(float seconds) {}
    virtual void draw(Graphics *g) {}

    virtual void onKeyPressed(QKeyEvent *event) {}
    virtual void onKeyReleased(QKeyEvent *event) {}
    virtual void onMousePressed(QMouseEvent *event) {}
    virtual void onMouseReleased(QMouseEvent *event) {}
    virtual void onMouseMoved(glm::vec2 &delta) {}
    virtual void onMouseWheelMoved(QWheelEvent *event) {}

protected:
    std::map<std::string, std::shared_ptr<GameObject>> m_gameObjects;
    TypeMap<std::shared_ptr<System>> m_systems;

};

#endif // WORLD_H
