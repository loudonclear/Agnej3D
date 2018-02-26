#include "EnemyCollider.h"
#include "engine/world/World.h"

EnemyCollider::EnemyCollider(GameObject *parent) : Component(parent), BoxCollider(parent)
{
}

void EnemyCollider::onCollide(Collision::ContactData cd) {
    if (cd.s2->getGameObject()->name == "player") {
        cd.s2->getTransform()->setPosition(glm::vec3(0, 40, 0));
    }
}

