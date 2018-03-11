#include "Player.h"
#include "engine/components/GraphicsComponent.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/physics/BoxCollider.h"
#include "engine/components/Transform.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/RigidBody.h"
#include "engine/components/SoundComponent.h"
#include "minecraft/PlayerMoveComponent.h"
#include "minecraft/MinecraftGameScreen.h"
#include "minecraft/HealthBar.h"
#include "engine/graphics/Camera.h"

Player::Player(const std::string &name) : GameObject(name)
{
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(this, "cube", "default", false);
    std::shared_ptr<UIComponent> ui = std::make_shared<HealthBar>(this, "green", glm::vec3(0.05f, 0.05f, 0.f), glm::vec3(0.3f, 0.05f, 1.f));

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(this);
    trans->setPosition(glm::vec3(0*32, 40.f, 0*32));
    trans->setScale(glm::vec3(0.8f, 1.8f, 0.8f));
    std::shared_ptr<Camera> c = MinecraftGameScreen::camera;
    c->setLook(glm::vec3(1, -1, 1));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<BoxCollider>(this);
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(this, false);
    std::shared_ptr<PlayerMoveComponent> pm = std::make_shared<PlayerMoveComponent>(this);
    std::shared_ptr<TickComponent> tc = pm;
    std::shared_ptr<InputComponent> in = pm;

    std::shared_ptr<SoundComponent> soundComp = std::make_shared<SoundComponent>(this);
    soundComp->setSource(QUrl::fromLocalFile(":/grass.wav"));
    soundComp->setVolume(0.f);
    soundComp->loop();
    soundComp->play();

    addComponent(ui);
    addComponent(comp);
    addComponent(soundComp);
    addComponent(tc);
    addComponent(in);
    addComponent(trans);
    addComponent(sc);
    addComponent(rb);
}
