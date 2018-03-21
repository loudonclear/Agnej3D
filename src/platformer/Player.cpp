#include "Player.h"

#include "engine/components/GraphicsComponent.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/physics/BoxCollider.h"
#include "engine/components/Transform.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/RigidBody.h"
#include "engine/graphics/Camera.h"
#include "engine/systems/OBJCollisionSystem.h"

#include "PlatformerScreen.h"
#include "platformer/PlayerMoveComponent.h"


Player::Player(const std::string &name, std::shared_ptr<OBJCollisionSystem> objc) : GameObject(name)
{
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(this, "sphere", "default", false);

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(this);
    trans->setPosition(glm::vec3(0, 15.f, 0));
    trans->setScale(glm::vec3(0.8f, 1.8f, 0.8f));
    std::shared_ptr<Camera> c = PlatformerScreen::camera;
    c->setEye(glm::vec3(0, 20, 0));
    c->setLook(glm::vec3(1, -1, 1));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<BoxCollider>(this);
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(this, false);
    std::shared_ptr<PlayerMoveComponent> pm = std::make_shared<PlayerMoveComponent>(this, objc);
    std::shared_ptr<TickComponent> tc = pm;
    std::shared_ptr<InputComponent> in = pm;

    addComponent(comp);
    addComponent(tc);
    addComponent(in);
    addComponent(trans);
    addComponent(sc);
    addComponent(rb);
}
