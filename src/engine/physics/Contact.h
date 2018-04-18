#ifndef CONTACT_H
#define CONTACT_H

#include <memory>
#include <glm/glm.hpp>
#include "Collision.h"

class ShapeCollider;

class ContactBasicData {
public:
    std::shared_ptr<ShapeCollider> c1, c2;

    glm::vec3 point;
    glm::vec3 normal;
    float penetration;

    //Collision::Triangle triangle;
};

class Contact : public ContactBasicData {
public:
    Contact() {}
   Contact(const ContactBasicData &data);

    bool isValid() const;

    bool invalidFlag;
    float desiredDeltaVelocity;
};

#endif // CONTACT_H
