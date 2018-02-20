#include "PlayerMoveComponent.h"
#include "engine/graphics/Camera.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/world/GameObject.h"
#include "engine/physics/RigidBody.h"

PlayerMoveComponent::PlayerMoveComponent(GameObject *parent) : TickComponent(parent)
{
}

void PlayerMoveComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_rigidbody = m_gameObject->getComponent<RigidBody>();
}

void PlayerMoveComponent::tick(float seconds) {
    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    glm::vec3 look = c->getLook();
    glm::vec3 up = glm::normalize(c->getUp());
    glm::vec3 dir = glm::normalize(glm::vec3(look.x, 0, look.z));
    glm::vec3 perp = glm::cross(up, dir);

    const float moveSpeed = 5.f;

    m_rigidbody->velocity = glm::vec3(0, -10, 0);


    if (Input::isKeyDown(Qt::Key_W)) {
        m_rigidbody->velocity += dir*moveSpeed;
    }
    if (Input::isKeyDown(Qt::Key_S)) {
        m_rigidbody->velocity += -dir*moveSpeed;
    }
    if (Input::isKeyDown(Qt::Key_D)) {
        m_rigidbody->velocity += -perp*moveSpeed;
    }
    if (Input::isKeyDown(Qt::Key_A)) {
        m_rigidbody->velocity += perp*moveSpeed;
    }
    if (Input::isKeyDown(Qt::Key_Space)) {
        m_rigidbody->velocity += up*moveSpeed*4.f;
    }

    c->setEye(m_transform->getPosition() + glm::vec3(0, 1, 0));
}
