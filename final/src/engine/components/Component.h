#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class GameObject;

class Component
{
public:
    Component(std::shared_ptr<GameObject> &parent) : m_gameObject(parent) {}
    virtual void init() {}

    std::shared_ptr<GameObject> getGameObject() { return m_gameObject; }

protected:

    std::shared_ptr<GameObject> m_gameObject;
};

#endif // COMPONENT_H
