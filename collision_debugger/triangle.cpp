#include "triangle.h"
#include "stdio.h"

Triangle::Triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    normal = glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]);
    normal = glm::normalize(normal);
}

Triangle Triangle::scale(glm::vec3 basis){
    return Triangle(vertices[0]*basis, vertices[1]*basis, vertices[2]*basis);
}
