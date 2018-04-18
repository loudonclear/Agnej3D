#include "TimingSystem.h"
#include "engine/components/TickComponent.h"
#include <algorithm>
#include <iostream>

TimingSystem::TimingSystem()
{
}

void TimingSystem::addGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<TickComponent> tickComponent = go->getComponent<TickComponent>();
    if (tickComponent != nullptr) m_tickComponents.push_back(tickComponent);
}

void TimingSystem::removeGameObject(const std::shared_ptr<GameObject> &go) {
    std::shared_ptr<TickComponent> tickComponent = go->getComponent<TickComponent>();
    if (tickComponent != nullptr)
        m_tickComponents.erase(std::remove(m_tickComponents.begin(), m_tickComponents.end(), tickComponent), m_tickComponents.end());
}

void TimingSystem::tick(float seconds) {
    for (auto iter = m_tickComponents.begin(); iter != m_tickComponents.end(); ++iter) {
        iter->get()->tick(seconds);
    }
}

void TimingSystem::fixedTick(float seconds) {
    for (auto iter = m_tickComponents.begin(); iter != m_tickComponents.end(); ++iter) {
        iter->get()->fixedTick(seconds);
    }
}

void TimingSystem::lateTick(float seconds) {
    for (auto iter = m_tickComponents.begin(); iter != m_tickComponents.end(); ++iter) {
        iter->get()->lateTick(seconds);
    }
}
