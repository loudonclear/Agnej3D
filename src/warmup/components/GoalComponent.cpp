#include "GoalComponent.h"

GoalComponent::GoalComponent(GameObject *parent, int &score) : Component(parent), BoxCollider(parent), score(score)
{
}

void GoalComponent::onCollide(Collision::ContactData cd) {
    if (cd.s2->getGameObject()->name == "ball") {
        score++;
        cd.s2->getTransform()->setPosition(glm::vec3(0, 0.5f, 0));
    }
}
