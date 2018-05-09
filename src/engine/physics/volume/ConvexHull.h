#ifndef CONVEXHULL_H
#define CONVEXHULL_H

#include <vector>
#include <glm/glm.hpp>

class ConvexHull
{
public:
    ConvexHull(std::vector<glm::vec3> vertices, std::vector<glm::ivec3> faces);

    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;

    glm::vec3 min, max;
    glm::vec3 com;
};

#endif // CONVEXHULL_H
