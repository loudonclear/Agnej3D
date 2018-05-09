#ifndef CONTACTGENERATOR_H
#define CONTACTGENERATOR_H

#include "physics/Contact.h"

#include <vector>
#include <array>
#include <list>

class ShapeCollider;
class Contact;
class RigidBody;

class ContactGenerator {
private:
    struct Arbiter {
        std::shared_ptr<ShapeCollider> colliders[2];
    };

    struct SupportPoint {
        glm::vec3 v;
        glm::vec3 localSupports[2];
        int id;

        inline SupportPoint(int id = 0) : id(id) {}
    };

    struct Simplex {
    public:
        SupportPoint _simplex[4];
        int num;
        SupportPoint &a;
        SupportPoint &b;
        SupportPoint &c;
        SupportPoint &d;

        inline Simplex() : a(_simplex[0]),b(_simplex[1]),c(_simplex[2]),d(_simplex[3]) { clear(); }

        inline void clear() { num = 0; }

        inline void set(SupportPoint a,SupportPoint b,SupportPoint c,SupportPoint d) { num = 4; this->a = a; this->b = b; this->c = c; this->d = d; }
        inline void set(SupportPoint a,SupportPoint b,SupportPoint c) { num = 3; this->a = a; this->b = b; this->c = c; }
        inline void set(SupportPoint a,SupportPoint b) { num = 2; this->a = a; this->b = b; }
        inline void set(SupportPoint a) { num = 1; this->a = a; }

        inline void push(SupportPoint p) { num = (std::min)(num+1,4); for(int i = num-1; i > 0; i--) _simplex[i] = _simplex[i-1]; _simplex[0] = p; }
    };

    struct Triangle {
        SupportPoint points[3];
        glm::vec3 n;

        inline Triangle(const SupportPoint &a,const SupportPoint &b,const SupportPoint &c) {
            points[0] = a;
            points[1] = b;
            points[2] = c;
            n = glm::normalize(glm::cross(b.v - a.v, c.v - a.v));
        }
    };
    struct Edge {
        SupportPoint points[2];

        inline Edge(const SupportPoint &a,const SupportPoint &b) {
            points[0] = a;
            points[1] = b;
        }
    };

    // from Real-Time Collision Detection
    static void barycentric(const glm::vec3 &p, const glm::vec3 &a, const glm::vec3 &b, const glm::vec3 &c, float *u, float *v, float *w) {
        glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
        float d00 = glm::dot(v0, v0);
        float d01 = glm::dot(v0, v1);
        float d11 = glm::dot(v1, v1);
        float d20 = glm::dot(v2, v0);
        float d21 = glm::dot(v2, v1);
        float denom = d00 * d11 - d01 * d01;
        *v = (d11 * d20 - d01 * d21) / denom;
        *w = (d00 * d21 - d01 * d20) / denom;
        *u = 1.0f - *v - *w;
    }

    static void processCollisionTests(std::vector<Arbiter> &pairs, std::vector<ContactBasicData> &newContactsThisFrame);

    static bool extrapolateContactInformation(ContactBasicData* contactData);
    static bool extrapolateContactInformation(const Triangle* triangle,
                                              ContactBasicData* contactData);

    static SupportPoint generateSupport(glm::vec3 directionWorld, std::shared_ptr<ShapeCollider> sca, std::shared_ptr<ShapeCollider> scb, int* idCounter = 0);

    static bool GJK(Simplex* simplex, ContactBasicData* contactData, int* idCounter);
    static bool EPA(const Simplex* simplex, ContactBasicData* contactData, int* idCounter);

    static void updatePersistentContacts(pool_vector<Contact> &contacts);

public:

    static void frame(std::vector<std::shared_ptr<RigidBody>> &bodies, pool_vector<Contact> &contacts);
};

#endif // CONTACTGENERATOR_H
