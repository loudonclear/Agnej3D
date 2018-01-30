#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>


struct Ray
{
public:
    Ray(glm::vec3 start, glm::vec3 direction) : start(start), direction(direction) {}

    glm::vec3 start;
    glm::vec3 direction;
};

#endif // RAY_H
