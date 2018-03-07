#include "Enemy.h"
#include "engine/components/GraphicsComponent.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/physics/BoxCollider.h"
#include "engine/components/Transform.h"
#include "engine/physics/ShapeCollider.h"
#include "engine/physics/RigidBody.h"
#include "minecraft/PlayerMoveComponent.h"
#include "minecraft/MinecraftGameScreen.h"
#include "minecraft/MinecraftEnemyAI.h"
#include "minecraft/EnemyCollider.h"
#include "minecraft/NPCText.h"

Enemy::Enemy(const std::string &name, const glm::vec3 &pos) : GameObject(name)
{
    std::shared_ptr<GraphicsComponent> comp = std::make_shared<GraphicsShapeComponent>(this, "cube", "enemy", false);

    std::shared_ptr<UIComponent> ui = std::make_shared<NPCText>(this, "default", glm::vec3(0, 1.1f, 0), "Ne!");

    std::shared_ptr<Transform> trans = std::make_shared<Transform>(this);
    trans->setPosition(pos);
    trans->setScale(glm::vec3(0.8f, 1.8f, 0.8f));

    std::shared_ptr<ShapeCollider> sc = std::make_shared<EnemyCollider>(this);
    std::shared_ptr<RigidBody> rb = std::make_shared<RigidBody>(this, false);
    std::shared_ptr<TickComponent> tc = std::make_shared<MinecraftEnemyAI>(this);

    addComponent(comp);
    addComponent(ui);
    addComponent(tc);
    addComponent(trans);
    addComponent(sc);
    addComponent(rb);
}
