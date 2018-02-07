#ifndef SYSTEM_H
#define SYSTEM_H

#include "engine/world/GameObject.h"
#include "engine/components/Component.h"

#include <vector>
#include <memory>


class System
{
public:
    virtual void addGameObject(const std::shared_ptr<GameObject> &go) = 0;
    virtual void removeGameObject(const std::shared_ptr<GameObject> &go) = 0;
};

#endif // SYSTEM_H
