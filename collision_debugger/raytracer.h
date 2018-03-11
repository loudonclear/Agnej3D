#ifndef RAYTRACER_H
#define RAYTRACER_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

struct HitTest
{
    float t;
    glm::vec3 hit;
    glm::vec3 normal;

    HitTest() : t(INFINITY) {}
};

class Raytracer
{
private:
    // Stores the viewport at the time of the constructor
    int viewport[4];

    // Stores the origin in world space at the time of the constructor
    glm::vec3 eye;

    // Stores a ray along each frustum corner at the time of the constructor
    glm::vec3 ray00, ray10, ray01, ray11;

public:
    // Read in the current modelview and projection matrices and remember them
    Raytracer();

    // Use the remembered information from the modelview and projection matrices
    // to compute the eye point and per-pixel rays
    glm::vec3 getEye() const { return eye; }
    glm::vec3 getRayForPixel(int x, int y) const;

    // Returns true if the ray starting at origin and traveling along ray hits
    // the AABB with coordinates from min to max. If true is returned, result
    // will be filled out with information about the collision.
    static bool hitTestCube(const glm::vec3 &min, const glm::vec3 &max, const glm::vec3 &origin, const glm::vec3 &ray, HitTest &result);
};

#endif // RAYTRACER_H
