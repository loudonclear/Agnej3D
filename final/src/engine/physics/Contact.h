#ifndef CONTACT_H
#define CONTACT_H

#include <memory>
#include <glm/glm.hpp>
#include "Collision.h"

class ShapeCollider;

class ContactBasicData {
public:
    glm::vec3 point;
    glm::vec3 normal;
    float penetration;

    //Collision::Triangle triangle;
};

class Contact : public ContactBasicData {
public:
    Contact() : invalidFlag(false) {}
    Contact(const ContactBasicData &data);

    std::shared_ptr<ShapeCollider> c[2];

    bool isValid() const;
    void calculateData();

    bool invalidFlag;
    float desiredDeltaVelocity;

    glm::mat4x4 matContactToWorld;
    glm::mat4x4 matWorldToContact;
    glm::vec3 closingVelocity_world;
    glm::vec3 closingVelocity_contact;
    glm::vec3 relativeContactPosition[2];
    float coefRestitution;
    float coefFriction;
};

#endif // CONTACT_H
