#include "ContactGenerator.h"

#include "Contact.h"
#include "ShapeCollider.h"
#include "RigidBody.h"
#include "components/Transform.h"

#include <numeric>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

const float minPenetration = -1.f;
const float maxContactDist = 0.05f;

glm::vec3 tripleCrossProduct(const glm::vec3 &v1, const glm::vec3 &v2) {
    return glm::cross(glm::cross(v1, v2), v1);
//    if (glm::length2(v1) <= FLOAT_EPSILON || glm::length2(v2) <= FLOAT_EPSILON) {
//        return glm::vec3(0, 0, 1);
//    }

//    glm::vec3 cro = glm::cross(v1, v2);
//    if (glm::length2(cro) <= FLOAT_EPSILON) {

//        cro = glm::cross(v1, glm::vec3(0, 0, 1));
//        if (glm::length2(cro) <= FLOAT_EPSILON) {
//            cro = glm::cross(v1, glm::vec3(0, 1, 0));
//        }
//    } else {
//        return glm::cross(cro, v1);
//    }

//    return cro;
}

void ContactGenerator::processCollisionTests(std::vector<Arbiter> &pairs, std::vector<ContactBasicData> &newContactsThisFrame) {

    for (Arbiter &pair : pairs) {
        ContactBasicData contactData;

        Simplex testSimplex;
        int id = 0;

        contactData.colliders[0] = pair.colliders[0];
        contactData.colliders[1] = pair.colliders[1];

        if (glm::length(pair.colliders[0]->getTransform()->getPosition() - pair.colliders[1]->getTransform()->getPosition()) > pair.colliders[0]->extent + pair.colliders[1]->extent) continue;

        if (GJK(&testSimplex, &contactData, &id) &&
            EPA(&testSimplex, &contactData, &id)) {

            newContactsThisFrame.emplace_back(std::move(contactData));
        }
    }
}

bool ContactGenerator::extrapolateContactInformation(ContactBasicData* contactData) {
    std::shared_ptr<Transform> transformA = contactData->colliders[0]->getTransform();
    std::shared_ptr<Transform> transformB = contactData->colliders[1]->getTransform();

    SupportPoint supports[3];

    for (int i = 0; i < 3; i++) {
        supports[i].localSupports[0] = contactData->triangleSupports_local[0][i];
        supports[i].localSupports[1] = contactData->triangleSupports_local[1][i];
        supports[i].v = (transformA->transformPoint(supports[i].localSupports[0])) - (transformB->transformPoint(supports[i].localSupports[1]));
    }

    Triangle triangle(supports[0],supports[1],supports[2]);
    return extrapolateContactInformation(&triangle,contactData);
}

bool ContactGenerator::extrapolateContactInformation(const Triangle* triangle, ContactBasicData* contactData) {
    std::shared_ptr<Transform> transformA = contactData->colliders[0]->getTransform();

    const float originDistance = glm::dot(triangle->n, triangle->points[0].v);

    float u, v, w;
    barycentric(triangle->n * originDistance, triangle->points[0].v, triangle->points[1].v,triangle->points[2].v, &u, &v, &w);

    if (!std::isfinite(u) || !std::isfinite(v) || !std::isfinite(w)) return false;
    if (fabs(u) > 1.0f || fabs(v) > 1.0f || fabs(w) > 1.0f) return false;


    contactData->point = u * transformA->transformPoint(triangle->points[0].localSupports[0]) +
                         v * transformA->transformPoint(triangle->points[1].localSupports[0]) +
                         w * transformA->transformPoint(triangle->points[2].localSupports[0]);
    contactData->normal = -triangle->n;
    contactData->penetration = originDistance;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 3; j++)
            contactData->triangleSupports_local[i][j] = triangle->points[j].localSupports[i];

    return true;
}

ContactGenerator::SupportPoint ContactGenerator::generateSupport(glm::vec3 dir, std::shared_ptr<ShapeCollider> sca, std::shared_ptr<ShapeCollider> scb, int* idCounter) {
    std::shared_ptr<Transform> transformA = sca->getTransform();
    std::shared_ptr<Transform> transformB = scb->getTransform();

    dir = glm::normalize(dir);

    SupportPoint ret(idCounter ? (*idCounter)++ : 0);
    ret.localSupports[0] = sca->getSupportPointLocal(dir);
    ret.localSupports[1] = scb->getSupportPointLocal(-dir);

    ret.v = (transformA->transformPoint(ret.localSupports[0])) - (transformB->transformPoint(ret.localSupports[1]));
    return ret;
}

#define gjk_simtest(v) (glm::dot(v, ao) > 0)
bool ContactGenerator::GJK(Simplex* simplex, ContactBasicData* contactData, int* idCounter) {
    Simplex& sim = *simplex;

    const unsigned _EXIT_ITERATION_LIMIT = 20;
    unsigned int _EXIT_ITERATION_NUM = 0;

    sim.clear();
    glm::vec3 dir = glm::vec3(0, 1, 0);
    SupportPoint s = generateSupport(dir,contactData->colliders[0], contactData->colliders[1], idCounter);
    if (fabs(glm::dot(dir, s.v)) >= glm::length(s.v)*0.8f) {
        dir = glm::vec3(1, 0, 0);
        s = generateSupport(dir, contactData->colliders[0], contactData->colliders[1], idCounter);
    }
    sim.push(s);
    dir = -s.v;

    while(true) {
        if (_EXIT_ITERATION_NUM++ >= _EXIT_ITERATION_LIMIT) return false;
        if (glm::length2(dir) <= FLOAT_EPSILON) {
            return false;
        }

        SupportPoint a = generateSupport(dir, contactData->colliders[0], contactData->colliders[1], idCounter);

        const float proj = glm::dot(a.v, dir);
        if (proj < 0) return false;
        sim.push(a);

        const glm::vec3 ao = -sim.a.v;
        bool _triangleSimplex_needToTestAboveBelow = true;

        if (sim.num == 2) {
            const glm::vec3 ab = sim.b.v - sim.a.v;
            dir = tripleCrossProduct(ab, ao);
            continue;
        } else if (sim.num == 3) {
            jmp_triangleSimplex:

            const glm::vec3 ab = sim.b.v - sim.a.v;
            const glm::vec3 ac = sim.c.v - sim.a.v;
            const glm::vec3 abc = glm::cross(ab, ac);

            if (gjk_simtest(glm::cross(ab, abc))) {
                sim.set(sim.a,sim.b);
                dir = tripleCrossProduct(ab, ao);
            } else {
                if (gjk_simtest(glm::cross(abc, ac))) {
                    sim.set(sim.a,sim.c);
                    dir = tripleCrossProduct(ac, ao);
                } else {
                    if (!_triangleSimplex_needToTestAboveBelow || gjk_simtest(abc)) {
                        dir = abc;
                    } else {
                        sim.set(sim.a,sim.c,sim.b);
                        dir = -abc;
                    }
                }
            }
            continue;
        } else {
            const glm::vec3 ab = sim.b.v - sim.a.v;
            const glm::vec3 ac = sim.c.v - sim.a.v;

            if (gjk_simtest(glm::cross(ab, ac))) {
                sim.num = 3;
                _triangleSimplex_needToTestAboveBelow = false;
                goto jmp_triangleSimplex;
            }

            const glm::vec3 ad = sim.d.v - sim.a.v;

            if (gjk_simtest(glm::cross(ac, ad))) {
                sim.set(sim.a,sim.c,sim.d);
                _triangleSimplex_needToTestAboveBelow = false;
                goto jmp_triangleSimplex;
            }

            if (gjk_simtest(glm::cross(ad, ab))) {
                sim.set(sim.a,sim.d,sim.b);
                _triangleSimplex_needToTestAboveBelow = false;
                goto jmp_triangleSimplex;
            }

            break;
        }
    }

    return true;
}
#undef gjk_simtest

bool ContactGenerator::EPA(const Simplex* simplex, ContactBasicData* contactData, int* idCounter) {
    const float _EXIT_GROWTH_THRESHOLD = 0.001f;
    const unsigned _EXIT_ITERATION_LIMIT = 30;
    unsigned int _EXIT_ITERATION_CUR = 0;

    std::list<Triangle> lst_triangles;
    std::list<Edge> lst_edges;

    auto lam_addEdge = [&](const SupportPoint &a,const SupportPoint &b)->void {
        for (auto it = lst_edges.begin(); it != lst_edges.end(); it++) {
            if (it->points[0].id==b.id && it->points[1].id==a.id) {
                lst_edges.erase(it);
                return;
            }
        }
        lst_edges.emplace_back(a,b);
    };

    lst_triangles.emplace_back(simplex->a,simplex->b,simplex->c);
    lst_triangles.emplace_back(simplex->a,simplex->c,simplex->d);
    lst_triangles.emplace_back(simplex->a,simplex->d,simplex->b);
    lst_triangles.emplace_back(simplex->b,simplex->d,simplex->c);

    while(true) {
        if (_EXIT_ITERATION_CUR++ >= _EXIT_ITERATION_LIMIT) return false;

        std::list<Triangle>::iterator closestTriangle_it;
        float closestTriangle_dst = FLT_MAX;
        for (auto it = lst_triangles.begin(); it != lst_triangles.end(); it++) {
            const float dst = glm::dot(it->n, it->points[0].v);
            if (dst < closestTriangle_dst) {
                closestTriangle_dst = dst;
                closestTriangle_it = it;
            }
        }

        if (!(closestTriangle_dst < FLT_MAX)) return false;
        const SupportPoint entry_new_support = generateSupport(closestTriangle_it->n, contactData->colliders[0], contactData->colliders[1], idCounter);

        const float newDst = glm::dot(closestTriangle_it->n, entry_new_support.v);
        const float growth = newDst - closestTriangle_dst;

        if ((growth < _EXIT_GROWTH_THRESHOLD)) {
            return extrapolateContactInformation(&*closestTriangle_it, contactData);
            break;
        }

        for (auto it = lst_triangles.begin(); it != lst_triangles.end();) {
            if (glm::dot(it->n, (entry_new_support.v - it->points[0].v)) > 0) {
                lam_addEdge(it->points[0],it->points[1]);
                lam_addEdge(it->points[1],it->points[2]);
                lam_addEdge(it->points[2],it->points[0]);
                it = lst_triangles.erase(it);
                continue;
            }
            it++;
        }

        for (auto it = lst_edges.begin(); it != lst_edges.end(); it++) {
            lst_triangles.emplace_back(entry_new_support,it->points[0],it->points[1]);
        }

        lst_edges.clear();
    }

    return false;
}

void ContactGenerator::updatePersistentContacts(pool_vector<Contact> &contacts) {

    for (Contact &c : contacts) {
        if (c.isValid()) {
            if ((c.colliders[0]->isStatic() || c.colliders[0]->getRigidBody() == nullptr || !c.colliders[0]->getRigidBody()->awake || c.colliders[0]->getRigidBody()->interaction) &&
                (c.colliders[1]->isStatic() || c.colliders[1]->getRigidBody() == nullptr || !c.colliders[1]->getRigidBody()->awake || c.colliders[1]->getRigidBody()->interaction)) {
                c.invalidate();
            } else {
                if (extrapolateContactInformation(&c)) {
                    if (c.penetration < minPenetration) {
                        c.invalidate();
                    } else if (glm::length2(c.point - c.initialContactPoint) > maxContactDist) {
                        c.invalidate();
                    }
                } else {
                    c.invalidate();
                }
            }
        }
    }
}


void ContactGenerator::frame(std::vector<std::shared_ptr<RigidBody>> &bodies, pool_vector<Contact> &contacts) {
    updatePersistentContacts(contacts);

    std::vector<Arbiter> procItems;
    std::vector<ContactBasicData> newContactsThisFrame;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        std::shared_ptr<ShapeCollider> const colliderA = bodies.at(i)->getCollider();
        for (unsigned int j = i+1; j < bodies.size(); j++) {
            std::shared_ptr<ShapeCollider> const colliderB = bodies.at(j)->getCollider();

            if (colliderA->getRigidBody()->interaction || colliderB->getRigidBody()->interaction) continue;

            if ((colliderA->isStatic() || !colliderA->getRigidBody()->awake) &&
                (colliderB->isStatic() || !colliderB->getRigidBody()->awake)) continue;

            procItems.emplace_back();
            auto& newItem = procItems.back();
            newItem.colliders[0] = colliderA;
            newItem.colliders[1] = colliderB;
        }
    }


    processCollisionTests(procItems, newContactsThisFrame);


    for (const auto& testData : newContactsThisFrame) {
        const auto colliderA = testData.colliders[0];
        const auto colliderB = testData.colliders[1];
        const auto colliderToCheck = colliderA->contactInvolvement.size() < colliderB->contactInvolvement.size() ? colliderA : colliderB;

        for (auto& contact : colliderToCheck->contactInvolvement) {
            if (!contact->isValid()) continue;

            if ((contact->colliders[0] == colliderA && contact->colliders[1] == colliderB) ||
                (contact->colliders[0] == colliderB && contact->colliders[1] == colliderA)) {
                if (glm::dot(contact->normal, testData.normal) > 0.97f) {
                    if (glm::length2(contact->point - testData.point) < 0.05f) {
                        contact->invalidate();
                        break;
                    }
                }
            }
        }

        contacts.emplace(std::move(testData));
    }
}
