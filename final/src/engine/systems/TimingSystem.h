#ifndef TIMINGSYSTEM_H
#define TIMINGSYSTEM_H

#include "System.h"

class TickComponent;

class TimingSystem : public System
{
public:
    TimingSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void tick(float seconds);
    void fixedTick(float seconds);
    void lateTick(float seconds);

private:
    std::vector<std::shared_ptr<TickComponent>> m_tickComponents;
};

#endif // TIMINGSYSTEM_H
