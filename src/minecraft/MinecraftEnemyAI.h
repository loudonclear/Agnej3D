#ifndef MINECRAFTENEMYAI_H
#define MINECRAFTENEMYAI_H


#include "engine/components/TickComponent.h"
#include "engine/components/InputComponent.h"
#include "engine/voxel/ChunkSystem.h"
#include "engine/physics/RigidBody.h"
#include "engine/physics/ShapeCollider.h"
#include "glm/glm.hpp"

class Transform;


class MinecraftEnemyAI : public TickComponent
{
public:
    MinecraftEnemyAI(GameObject *parent);

    void tick(float seconds);
    void init();

private:
    std::shared_ptr<Transform> m_target;
    std::shared_ptr<Transform> m_transform;
    std::shared_ptr<RigidBody> m_rigidbody;
    std::shared_ptr<ShapeCollider> m_shapeCollider;

    std::shared_ptr<ChunkSystem> m_chunkManager;
};

#endif // MINECRAFTENEMYAI_H
