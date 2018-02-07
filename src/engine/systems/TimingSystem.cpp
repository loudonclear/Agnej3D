#include "TimingSystem.h"
#include "engine/components/TickComponent.h"
#include "engine/components/FixedTickComponent.h"
#include <algorithm>
#include <iostream>

TimingSystem::TimingSystem()
{
}

void TimingSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<TickComponent> tickComponent = go->getComponent<TickComponent>();
    if (tickComponent != nullptr) m_tickComponents.push_back(tickComponent);

    std::shared_ptr<FixedTickComponent> fixedTickComponent = go->getComponent<FixedTickComponent>();
    if (fixedTickComponent != nullptr) m_fixedTickComponents.push_back(fixedTickComponent);
}

void TimingSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<TickComponent> tickComponent = go->getComponent<TickComponent>();
    if (tickComponent != nullptr)
        m_tickComponents.erase(std::remove(m_tickComponents.begin(), m_tickComponents.end(), tickComponent), m_tickComponents.end());

    std::shared_ptr<FixedTickComponent> fixedTickComponent = go->getComponent<FixedTickComponent>();
    if (fixedTickComponent != nullptr)
        m_fixedTickComponents.erase(std::remove(m_fixedTickComponents.begin(), m_fixedTickComponents.end(), fixedTickComponent), m_fixedTickComponents.end());
}

void TimingSystem::tick(float seconds) {
    for (auto iter = m_tickComponents.begin(); iter != m_tickComponents.end(); ++iter) {
        iter->get()->tick(seconds);
    }
}

void TimingSystem::fixedTick(float seconds) {
    for (auto iter = m_fixedTickComponents.begin(); iter != m_fixedTickComponents.end(); ++iter) {
        iter->get()->fixedTick(seconds);
    }
}
