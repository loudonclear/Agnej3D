#ifndef COLLISION_H
#define COLLISION_H


#include <memory>
#include <glm/glm.hpp>

class ShapeCollider;

class Collision
{
public:

    struct ContactData {
        ShapeCollider *s1;
        ShapeCollider *s2;

        glm::vec3 contactPoint;
        glm::vec3 contactNormal;
        float penetration;
    };

    static bool collide(ContactData &cd);

private:

    struct SupportPoint {
        glm::vec3 v;
        glm::vec3 supports[2];

        int uniqueId;

        inline SupportPoint(int uniqueId = 0) : uniqueId(uniqueId) {}
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

            inline void set(SupportPoint a) { num = 1; this->a = a; }
            inline void set(SupportPoint a, SupportPoint b) { num = 2; this->a = a; this->b = b; }
            inline void set(SupportPoint a, SupportPoint b, SupportPoint c) { num = 3; this->a = a; this->b = b; this->c = c; }
            inline void set(SupportPoint a, SupportPoint b, SupportPoint c, SupportPoint d) { num = 4; this->a = a; this->b = b; this->c = c; this->d = d; }

            inline void add(SupportPoint p) { num = (std::min)(num+1,4); for(int i = num-1; i > 0; i--) _simplex[i] = _simplex[i-1]; _simplex[0] = p; }
    };

    struct Edge {
        SupportPoint points[2];

        inline Edge(const SupportPoint &a, const SupportPoint &b) {
            points[0] = a;
            points[1] = b;
        }
    };

    struct Triangle {
        SupportPoint points[3];
        glm::vec3 n;

        inline Triangle(const SupportPoint &a, const SupportPoint &b, const SupportPoint &c) {
            points[0] = a;
            points[1] = b;
            points[2] = c;
            n = glm::normalize(glm::cross(b.v - a.v, c.v - a.v));
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

    static bool gjk(ShapeCollider *s1, ShapeCollider *s2, Simplex &simplex, int *uniqueIdCounter);
    static bool epa(ShapeCollider *s1, ShapeCollider *s2, Simplex &simplex, int *uniqueIdCounter, ContactData &contactData);
    static SupportPoint generateSupport(ShapeCollider *s1, ShapeCollider *s2, glm::vec3 &dir, int *uniqueIdCounter = 0);
    static bool extrapolateContactInfo(const Triangle *triangle, ContactData &contactData);
};

#endif // COLLISION_H
