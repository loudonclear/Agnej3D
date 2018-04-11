#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "engine/components/Component.h"
#include "engine/physics/Collision.h"
#include "engine/components/Transform.h"

class RigidBody : public Component
{
public:
    RigidBody(std::shared_ptr<GameObject> &parent, bool isStatic);

    void init();
    bool isStatic() { return m_isStatic; }
    virtual void onCollide(Collision::ContactData cd);

protected:

    std::shared_ptr<Transform> m_transform;

private:
    bool m_isStatic;
};

#endif // RIGIDBODY_H
