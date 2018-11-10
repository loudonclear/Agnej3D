#include "ConvexHull.h"

ConvexHull::ConvexHull(std::vector<glm::vec3> vertices, std::vector<glm::ivec3> faces) : vertices(vertices), faces(faces)
{
    glm::vec3 tot;
    for (const glm::vec3 &v : vertices) {
        for (int i = 0; i < 3; i++) {
            min[i] = glm::min(v[i], min[i]);
            max[i] = glm::max(v[i], max[i]);
        }
        tot += v;
    }

    com = tot / static_cast<float>(vertices.size());
}


