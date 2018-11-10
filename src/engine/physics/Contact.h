#ifndef CONTACT_H
#define CONTACT_H


#include <vector>
#include <glm/glm.hpp>
#include "pool_vector.h"
#include <memory>

class ShapeCollider;

class ContactBasicData {
public:
    std::shared_ptr<ShapeCollider> colliders[2];

    glm::vec3 point;
    glm::vec3 normal;
	float penetration;

    glm::vec3 triangleSupports_local[2][3];
};

class Contact : public ContactBasicData {

public:
    Contact(const ContactBasicData& data);

    Contact(Contact&& other);

    ~Contact();

    bool isValid() const;
    void invalidate();

    void calcData();


    glm::vec3 initialContactPoint;
    glm::vec3 initialNormal;

    glm::mat4x4 matContactToWorld;
    glm::mat4x4 matWorldToContact;
    glm::vec3 closingVelocity_world;
    glm::vec3 closingVelocity_contact;
    glm::vec3 relativeContactPosition[2];
	float coefRestitution;
	float coefFriction;
    float desiredDeltaVelocity;

private:
    bool invalidFlag;
    pool_vector<Contact*>::iterator colliderIterators[2];

};

#endif // CONTACT_H
