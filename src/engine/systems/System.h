#ifndef SYSTEM_H
#define SYSTEM_H

#include "engine/world/GameObject.h"
#include "engine/components/Component.h"

#include <vector>
#include <memory>


class System
{
public:
    System();

    virtual void addGameObject(GameObject go) = 0;
    virtual void removeGameObject(GameObject go) = 0;


};

#endif // SYSTEM_H
