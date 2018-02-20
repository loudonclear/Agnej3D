#ifndef COMPONENT_H
#define COMPONENT_H

#include <memory>

class GameObject;

class Component
{
public:
    Component(GameObject *parent) : m_gameObject(parent) {}
    virtual void init() {}

    GameObject *getGameObject() { return m_gameObject; }

protected:

    GameObject *m_gameObject;
};

#endif // COMPONENT_H
