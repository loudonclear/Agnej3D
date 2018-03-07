#include "PlayerMoveComponent.h"
#include "engine/graphics/Camera.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/input/Input.h"
#include "engine/components/Transform.h"
#include "engine/world/GameObject.h"
#include "engine/physics/RigidBody.h"
#include "engine/components/SoundComponent.h"
#include "engine/components/InputComponent.h"
#include "minecraft/HealthBar.h"
#include "engine/world/World.h"
#include <iostream>

PlayerMoveComponent::PlayerMoveComponent(GameObject *parent) : Component(parent), TickComponent(parent), InputComponent(parent)
{
}

void PlayerMoveComponent::init() {
    m_transform = m_gameObject->getComponent<Transform>();
    m_rigidbody = m_gameObject->getComponent<RigidBody>();
    m_shapeCollider = m_gameObject->getComponent<ShapeCollider>();
    m_chunkManager = m_gameObject->getWorld()->getSystem<ChunkSystem>();
    m_soundComponent = m_gameObject->getComponent<SoundComponent>();

    m_camera = MinecraftGameScreen::camera;
    m_rigidbody->force = glm::vec3(0, -20, 0);
}

void PlayerMoveComponent::tick(float seconds) {
    glm::vec3 look = m_camera->getLook();
    glm::vec3 up = glm::normalize(m_camera->getUp());
    glm::vec3 dir = glm::normalize(glm::vec3(look.x, 0, look.z));
    glm::vec3 perp = glm::cross(up, dir);

    const float moveSpeed = 7.5f;

    m_rigidbody->velocity.x = 0; m_rigidbody->velocity.z = 0;

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
        m_rigidbody->velocity.y = 7.5f;
        m_rigidbody->isGrounded = false;
    }


    if (!m_rigidbody->isGrounded || (m_rigidbody->velocity.x == 0 && m_rigidbody->velocity.z == 0)) {
        m_soundComponent->setVolume(0.f);
    } else {
        m_soundComponent->setVolume(0.5f);
    }

     m_chunkManager->sweep(seconds, m_shapeCollider);
}

void PlayerMoveComponent::fixedTick(float seconds) {

}

void PlayerMoveComponent::lateTick(float seconds) {
    m_camera->setEye(m_transform->getPosition() + glm::vec3(0, 0.5f, 0));
    glm::vec3 look = m_camera->getLook();
    m_camera->translate(-look*zoom);
}

void PlayerMoveComponent::onMouseReleased(QMouseEvent *event) {
    glm::ivec3 position;
    glm::vec3 normal;
    if (m_chunkManager->raycast(m_camera->getEye(), m_camera->getLook(), 20, position, normal)) {
        std::map<char, Block> *blockTypes = m_chunkManager->getBlockTypes();

        if (event->button() == Qt::LeftButton) {
            m_chunkManager->setBlock(position.x, position.y, position.z, blockTypes->find(0)->second);
        } else if (event->button() == Qt::RightButton) {
            position += glm::ivec3(normal);
            glm::vec3 eye = m_camera->getEye();
            glm::vec3 look = m_camera->getLook();
            glm::vec3 dir = glm::vec3(eye.x - position.x, 0, eye.z - position.z);
            if (dir.x != 0 || dir.z != 0) dir = glm::normalize(dir);

            if (!m_shapeCollider->pointInside(glm::vec3(position) + glm::vec3(0, -glm::sign(look.y) * .5f, 0) + 0.51f * dir)) {
                m_chunkManager->setBlock(position.x, position.y, position.z, blockTypes->find(3)->second);
            }
        }
    }
}

void PlayerMoveComponent::onMouseMoved(glm::vec2 &delta) {
    m_camera->rotate(-delta.x / 100.f, -delta.y / 100.f);
}

void PlayerMoveComponent::onMouseWheelMoved(QWheelEvent *event) {
    zoom += -event->delta() / 20.f;
    zoom = glm::clamp(zoom, 0.1f, 3.f);
}
