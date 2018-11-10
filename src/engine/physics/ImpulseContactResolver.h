#ifndef IMPULSECONTACTRESOLVER_H
#define IMPULSECONTACTRESOLVER_H

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "pool_vector.h"

class Contact;

class ImpulseContactResolver {
protected:

    static float deltaVelocity(Contact* contact);
    static glm::vec3 calculateImpulse(Contact* contact);

    static void buildContactClusters(pool_vector<Contact> &contacts, std::list<std::list<Contact*>> &contactClusters);
    static void resolveContactCluster(std::list<Contact*> &cluster);
public:

    static void frame(pool_vector<Contact> &contacts);
};

#endif // IMPULSECONTACTRESOLVER_H
