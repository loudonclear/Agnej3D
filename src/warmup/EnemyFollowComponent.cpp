#include "EnemyFollowComponent.h"
#include "engine/world/GameObject.h"
#include "engine/world/World.h"


EnemyFollowComponent::EnemyFollowComponent(std::shared_ptr<GameObject> &parent) : TickComponent(parent)
{
}

void EnemyFollowComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_target = m_gameObject->getWorld()->getGameObject("ball")->getComponent<Transform>();
}

void EnemyFollowComponent::tick(float seconds) {
    glm::vec3 dir;

    if (m_target->getPosition().x > m_transform->getPosition().x - 0.75f) {
        dir = glm::vec3(1, 0, 0);

        if (fabs(m_target->getPosition().z - m_transform->getPosition().z) < 1.1f) {
            dir.z = m_transform->getPosition().z < 0 ? -1.f : 1.f;
        }
    } else {
        if (fabs(m_target->getPosition().z - m_transform->getPosition().z) > 0.3f) {
            dir.z = m_transform->getPosition().z < m_target->getPosition().z ? 1.f : -1.f;
        } else {
            dir = glm::normalize(m_target->getPosition() - m_transform->getPosition());
        }
    }
    m_transform->translate(dir * seconds * 2.f);
}
