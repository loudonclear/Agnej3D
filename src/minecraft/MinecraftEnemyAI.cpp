#include "MinecraftEnemyAI.h"
#include "engine/world/GameObject.h"
#include "engine/world/World.h"

MinecraftEnemyAI::MinecraftEnemyAI(GameObject *parent) : Component(parent), TickComponent(parent)
{
}

void MinecraftEnemyAI::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_rigidbody = m_gameObject->getComponent<RigidBody>();
    m_shapeCollider = m_gameObject->getComponent<ShapeCollider>();
    m_chunkManager = m_gameObject->getWorld()->getSystem<ChunkManager>();
    m_target = m_gameObject->getWorld()->getGameObject("player")->getComponent<Transform>();

    m_rigidbody->force = glm::vec3(0, -20, 0);
}

void MinecraftEnemyAI::tick(float seconds) {


    const float moveSpeed = 3.f;

    glm::vec3 dir = m_target->getPosition() - m_transform->getPosition();
    m_rigidbody->velocity.x = 0; m_rigidbody->velocity.z = 0;
    m_rigidbody->velocity += moveSpeed * glm::normalize(glm::vec3(dir.x, 0, dir.z));

    if (m_rigidbody->isGrounded) {
        m_rigidbody->velocity.y = 7.5f;
        m_rigidbody->isGrounded = false;
    }

    m_chunkManager->sweep(seconds, m_shapeCollider);
}

