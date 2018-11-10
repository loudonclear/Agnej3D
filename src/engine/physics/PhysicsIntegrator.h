#ifndef PHYSICSINTEGRATOR_H
#define PHYSICSINTEGRATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <memory>

class RigidBody;

class PhysicsIntegrator {
protected:

    static void updateSleepWake(std::shared_ptr<RigidBody> &body, const glm::vec3& deltaPosition, const glm::vec3& deltaRotation, float dt);
};

#endif // PHYSICSINTEGRATOR_H
