#include "Player.h"
#include "engine/components/GraphicsComponent.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/physics/CylinderCollider.h"
#include "engine/components/Transform.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/RigidBody.h"
#include "minecraft/PlayerMoveComponent.h"
#include "minecraft/MinecraftGameScreen.h"
#include "engine/graphics/Camera.h"

Player::Player(const std::string &name) : GameObject(name)
{
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(this, "cylinder", "default", false);

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(this);
    trans->setPosition(glm::vec3(5*32, 32.f, 5*32));
    trans->setScale(glm::vec3(1, 2, 1));
    //std::shared_ptr<Camera> c = MinecraftGameScreen::camera;

    std::shared_ptr<ShapeCollider> sc = std::make_shared<CylinderCollider>(this);
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(this, false);
    std::shared_ptr<TickComponent> tc = std::make_shared<PlayerMoveComponent>(this);

    addComponent(comp);
    addComponent(tc);
    addComponent(trans);
    addComponent(sc);
    addComponent(rb);
}
