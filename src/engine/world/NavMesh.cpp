#include "NavMesh.h"

#include <iostream>
#include "engine/graphics/Graphics.h"

NavMesh::NavMesh(std::shared_ptr<OBJ> m) : m_mesh(m), graph(INT_MAX) {    

    for (int i = 0; i < m_mesh->vertices.size(); i++) {
        graph.addVertex(i);
    }

    QList<OBJ::Triangle*> tris = m_mesh->triangles;
    for (int i = 0; i < tris.size(); i++) {
        OBJ::Triangle *t = tris.at(i);
        int a = t->a.vertexIndex;
        int b = t->b.vertexIndex;
        int c = t->c.vertexIndex;

        glm::vec3 v0 = t->vertices[0];
        glm::vec3 v1 = t->vertices[1];
        glm::vec3 v2 = t->vertices[2];

        glm::vec3 m0 = (v0 + v1) / 2.f;
        glm::vec3 m1 = (v1 + v2) / 2.f;
        glm::vec3 m2 = (v2 + v0) / 2.f;

        graph.addEdge(a, b, glm::distance(m0, m1));
        graph.addEdge(b, a, glm::distance(m0, m1));
        graph.addEdge(b, c, glm::distance(m1, m2));
        graph.addEdge(c, b, glm::distance(m1, m2));
        graph.addEdge(c, a, glm::distance(m2, m0));
        graph.addEdge(a, c, glm::distance(m2, m0));

        vertmap[t->a.vertexIndex] = v0;
        vertmap[t->b.vertexIndex] = v1;
        vertmap[t->c.vertexIndex] = v2;
    }

}

OBJ::Triangle* NavMesh::getTriangle(Ray r, RaycastResult &res) {
    OBJ::Triangle* found = nullptr;
    float best = FLT_MAX;

    for (OBJ::Triangle *t : m_mesh->triangles) {
        TriangleCollider tri(t->vertices[0], t->vertices[1], t->vertices[2]);
        RaycastResult temp;
        if (tri.raycast(r, nullptr, temp)) {

            if (temp.t > 0.f && temp.t < best) {
                best = temp.t;
                found = t;
                res = temp;
            }
        }
    }

    return found;
}

void NavMesh::drawPath(Graphics *g) {
    for (Edge<int, float> edge : path) {
        glm::vec3 start = vertmap[edge.start->data];
        glm::vec3 end = vertmap[edge.end->data];

        g->clearTransform();
        g->translate((start + end) / 2.f);
        g->drawShape("sphere");
    }
}

void NavMesh::getPath(OBJ::Triangle *start, OBJ::Triangle *end) {

    path = graph.aStar(start->a.vertexIndex, end->a.vertexIndex);

    //std::vector<OBJ::Triangle*> triPath;
//    for (auto edge = path.begin(); edge != path.end(); edge++) {

//        glm::vec3 s = vertmap[edge->start->data];
//        glm::vec3 e = vertmap[edge->end->data];
//        //auto next = std::next(edge);
//        //std::set<int> ts = {edge->start.data, edge->end.data, next->end.data};
//        //triPath.push_back(triMap[ts]);
//    }

//    std::cout << std::endl;
}
