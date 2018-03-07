#include "EnemyCollider.h"
#include "engine/world/World.h"
#include "minecraft/HealthBar.h"

EnemyCollider::EnemyCollider(GameObject *parent) : Component(parent), BoxCollider(parent)
{
}

void EnemyCollider::onCollide(Collision::ContactData cd) {
    if (cd.s2->getGameObject()->name == "player") {
        cd.s2->getTransform()->setPosition(glm::vec3(0, 40, 0));
        cd.s2->getRigidBody()->velocity = glm::vec3(0, 0, 0);
        std::shared_ptr<HealthBar> hb = std::dynamic_pointer_cast<HealthBar>(cd.s2->getGameObject()->getComponent<UIComponent>());
        hb->setHealth(hb->getHealth() - 0.2f);
    }
}

