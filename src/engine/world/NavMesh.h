#ifndef NAVMESH_H
#define NAVMESH_H

#include <memory>
#include <map>
#include <set>
#include "engine/graphics/Obj.h"
#include "engine/physics/TriangleCollider.h"
#include "Graph.h"

class Graphics;

class NavMesh
{
public:
    NavMesh(std::shared_ptr<OBJ> m);

    OBJ::Triangle *getTriangle(Ray r, RaycastResult &res);
    void getPath(OBJ::Triangle *start, OBJ::Triangle *end);
    void drawPath(Graphics *g);

private:
    std::shared_ptr<OBJ> m_mesh;
    Graph<int, float> graph;
    std::map<int, glm::vec3> vertmap;
    std::list<Edge<int, float>> path;

};

#endif // NAVMESH_H
