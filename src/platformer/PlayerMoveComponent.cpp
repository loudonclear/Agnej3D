#include "PlayerMoveComponent.h"

#include <iostream>

#include "engine/graphics/Camera.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/world/GameObject.h"
#include "engine/physics/RigidBody.h"
#include "engine/components/InputComponent.h"
#include "engine/world/World.h"
#include "engine/systems/OBJCollisionSystem.h"

#include "PlatformerScreen.h"


PlayerMoveComponent::PlayerMoveComponent(GameObject *parent, std::shared_ptr<OBJCollisionSystem> objc) : Component(parent), TickComponent(parent), InputComponent(parent), m_objCollisionSystem(objc)
{
}

void PlayerMoveComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_rigidbody = m_gameObject->getComponent<RigidBody>();
    m_shapeCollider = m_gameObject->getComponent<ShapeCollider>();

    m_camera = PlatformerScreen::camera;
    m_rigidbody->force = glm::vec3(0, -20, 0);
}

void PlayerMoveComponent::tick(float seconds) {
    glm::vec3 look = m_camera->getLook();
    glm::vec3 up = glm::normalize(m_camera->getUp());
    glm::vec3 dir = glm::normalize(glm::vec3(look.x, 0, look.z));
    glm::vec3 perp = glm::cross(up, dir);

    const float moveSpeed = 7.5f;
    m_rigidbody->force = glm::vec3(0, -10, 0);
    m_rigidbody->velocity += m_rigidbody->force * seconds;

    m_rigidbody->velocity.x = 0; m_rigidbody->velocity.z = 0;
    //m_rigidbody->velocity.y = 0;

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
    if (Input::isKeyDown(Qt::Key_Space) && m_rigidbody->isGrounded) {
        m_rigidbody->velocity.y += 7.5f;
        m_rigidbody->isGrounded = false;
    }
//    if (Input::isKeyDown(Qt::Key_Shift)) {
//        m_rigidbody->velocity.y -= 7.5f;
//    }

    m_objCollisionSystem->collide(m_shapeCollider, seconds);
}

void PlayerMoveComponent::fixedTick(float seconds) {

}

void PlayerMoveComponent::lateTick(float seconds) {
    m_camera->setEye(m_transform->getPosition() + glm::vec3(0, 0.5f, 0));
    glm::vec3 look = m_camera->getLook();
    m_camera->translate(-look*zoom);
}

void PlayerMoveComponent::onMouseMoved(glm::vec2 &delta) {
    m_camera->rotate(-delta.x / 100.f, -delta.y / 100.f);
}

void PlayerMoveComponent::onMouseWheelMoved(QWheelEvent *event) {
    zoom += -event->delta() / 20.f;
    zoom = glm::clamp(zoom, 0.1f, 3.f);
}
