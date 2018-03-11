#include "raytracer.h"
#include <qgl.h>
#include <GL/glu.h>

/*
 * NOTE: You don't need to change anything here.
 */

static glm::vec3 unProject(float winX, float winY, double *modelview, double *projection, int *viewport)
{
    double objX, objY, objZ;
    gluUnProject(winX, winY, 1, modelview, projection, viewport, &objX, &objY, &objZ);
    return glm::vec3(objX, objY, objZ);
}

static inline glm::vec3 lerp(const glm::vec3 &a, const glm::vec3 &b, float fraction) { return a + (b - a) * fraction; }

Raytracer::Raytracer()
{
    // Read camera information from OpenGL
    double modelview[16];
    double projection[16];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    // Reconstruct the eye position
    glm::vec3 xaxis(modelview[0], modelview[1], modelview[2]);
    glm::vec3 yaxis(modelview[4], modelview[5], modelview[6]);
    glm::vec3 zaxis(modelview[8], modelview[9], modelview[10]);
    glm::vec3 offset(modelview[12], modelview[13], modelview[14]);
    eye = -glm::vec3(glm::dot(offset, xaxis), glm::dot(offset, yaxis), glm::dot(offset, zaxis));

    // Generate the four corner rays
    int xmin = viewport[0];
    int ymin = viewport[1];
    int xmax = xmin + viewport[2] - 1;
    int ymax = ymin + viewport[3] - 1;
    ray00 = unProject(xmin, ymin, modelview, projection, viewport) - eye;
    ray10 = unProject(xmax, ymin, modelview, projection, viewport) - eye;
    ray01 = unProject(xmin, ymax, modelview, projection, viewport) - eye;
    ray11 = unProject(xmax, ymax, modelview, projection, viewport) - eye;
}

glm::vec3 Raytracer::getRayForPixel(int x, int y) const
{
    // Construct the per-pixel ray using linear interpolation from the rays
    // along each corner of the frustum (the origin is the eye point)
    float fx = (float)(x - viewport[0]) / (float)viewport[2];
    float fy = 1 - (float)(y - viewport[1]) / (float)viewport[3];
    glm::vec3 ray0 = lerp(ray00, ray10, fx);
    glm::vec3 ray1 = lerp(ray01, ray11, fx);
    return glm::normalize(lerp(ray0, ray1, fy));
}

bool Raytracer::hitTestCube(const glm::vec3 &cubeMin, const glm::vec3 &cubeMax, const glm::vec3 &origin, const glm::vec3 &ray, HitTest &result)
{
    // This uses the slab intersection method
    glm::vec3 tMin = (cubeMin - origin) / ray;
    glm::vec3 tMax = (cubeMax - origin) / ray;
    glm::vec3 t1 = glm::vec3(fminf(tMin.x, tMax.x), fminf(tMin.y, tMax.y), fminf(tMin.z, tMax.z));
    glm::vec3 t2 = glm::vec3(fmaxf(tMin.x, tMax.x), fmaxf(tMin.y, tMax.y), fmaxf(tMin.z, tMax.z));
    float tNear = fmaxf(t1.x, fmaxf(t1.y, t1.z));
    float tFar = fminf(t2.x, fminf(t2.y, t2.z));

    // Pick the intersection with the front faces (not the back faces)
    if (tNear > 0 && tNear < tFar) {
        const float epsilon = 1.0e-6;
        result.hit = origin + ray * tNear;
        if (result.hit.x < cubeMin.x + epsilon) result.normal = glm::vec3(-1, 0, 0);
        else if (result.hit.y < cubeMin.y + epsilon) result.normal = glm::vec3(0, -1, 0);
        else if (result.hit.z < cubeMin.z + epsilon) result.normal = glm::vec3(0, 0, -1);
        else if (result.hit.x > cubeMax.x - epsilon) result.normal = glm::vec3(1, 0, 0);
        else if (result.hit.y > cubeMax.y - epsilon) result.normal = glm::vec3(0, 1, 0);
        else result.normal = glm::vec3(0, 0, 1);
        result.t = tNear;
        return true;
    }

    return false;
}
