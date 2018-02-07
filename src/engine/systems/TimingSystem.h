#ifndef TIMINGSYSTEM_H
#define TIMINGSYSTEM_H

#include "System.h"

class TickComponent;
class FixedTickComponent;

class TimingSystem : public System
{
public:
    TimingSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void tick(float seconds);
    void fixedTick(float seconds);

private:
    std::vector<std::shared_ptr<TickComponent>> m_tickComponents;
    std::vector<std::shared_ptr<FixedTickComponent>> m_fixedTickComponents;
};

#endif // TIMINGSYSTEM_H
