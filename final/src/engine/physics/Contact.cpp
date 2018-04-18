#include "Contact.h"
#include <glm/gtx/orthonormalize.hpp>
#include "ShapeCollider.h"

Contact::Contact(const ContactBasicData &data) : ContactBasicData(data), invalidFlag(false)
{
}

bool Contact::isValid() const {
    if (invalidFlag) return false;

    return true;
}

void Contact::calculateData() {
    matContactToWorld = glm::mat4x4(glm::orthonormalize(glm::mat3x3(normal.x, 0, 0,
                                                                    normal.y, 1, 0,
                                                                    normal.z, 0, 1)));

    closingVelocity_world = glm::vec3(0, 0, 0);
    closingVelocity_contact = glm::vec3(0, 0, 0);

    for (int i = 0; i < 2; i++) {
        relativeContactPosition[i] = point - c[i]->getCenterOfMass();

        auto rigidBody = c[i]->getRigidBody();

        const glm::vec3& bodyLinearVelocity = rigidBody->velocity;
        const glm::vec3& bodyAngularVelocity = rigidBody->angularVelocity;
        const glm::vec3& lastFrameAcceleration = rigidBody->lastAcceleration;
        const glm::vec3& lastFramePlanarAcceleration = lastFrameAcceleration - (glm::dot(lastFrameAcceleration, normal) * normal);
        const glm::vec3& localVelocity_world =
            glm::cross(bodyAngularVelocity, relativeContactPosition[i]) +
            bodyLinearVelocity +
            lastFramePlanarAcceleration;

        const float sign = (i == 0 ? -1.0f : 1.0f);

        closingVelocity_world += sign * localVelocity_world;
    }

    closingVelocity_contact = closingVelocity_world * glm::mat3x3(matWorldToContact);

    coefRestitution = 0.2f;
    coefFriction = 0.8f;
}
