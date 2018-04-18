#include "FinalApplication.h"

#include "FinalMenuScreen.h"
#include "FinalDebugScreen.h"
#include "engine/graphics/Material.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Shape.h"

#include <iostream>

FinalApplication::FinalApplication()
{
    Graphics *graphics = Graphics::getGlobalInstance();

    Material grass;
    grass.textureName = "grass";
    grass.textureRepeat = glm::vec2(10, 10);
    graphics->addMaterial("grass", grass);

    std::vector<float> verts;
    glm::vec3 v0 = glm::vec3(0, 0.82f, 0);
    glm::vec3 n0 = glm::vec3(0, 1, 0);
    glm::vec2 t0 = glm::vec2(0, 0);

    glm::vec3 v1 = glm::vec3(0.5f, 0, -0.3f);
    glm::vec3 n1 = glm::normalize(glm::vec3(0.5f, 0, -0.3f));
    glm::vec2 t1 = glm::vec2(0, 0);

    glm::vec3 v2 = glm::vec3(-0.5f, 0, -0.3f);
    glm::vec3 n2 = glm::normalize(glm::vec3(-0.5f, 0, -0.3f));
    glm::vec2 t2 = glm::vec2(0, 0);

    glm::vec3 v3 = glm::vec3(0, 0, 0.6);
    glm::vec3 n3 = glm::vec3(0, 0, 1);
    glm::vec2 t3 = glm::vec2(0, 0);


    verts.push_back(v0.x); verts.push_back(v0.y); verts.push_back(v0.z);
    verts.push_back(n0.x); verts.push_back(n0.y); verts.push_back(n0.z);
    verts.push_back(t0.x); verts.push_back(t0.y);
    verts.push_back(v1.x); verts.push_back(v1.y); verts.push_back(v1.z);
    verts.push_back(n1.x); verts.push_back(n1.y); verts.push_back(n1.z);
    verts.push_back(t1.x); verts.push_back(t1.y);
    verts.push_back(v2.x); verts.push_back(v2.y); verts.push_back(v2.z);
    verts.push_back(n2.x); verts.push_back(n2.y); verts.push_back(n2.z);
    verts.push_back(t2.x); verts.push_back(t2.y);

    verts.push_back(v0.x); verts.push_back(v0.y); verts.push_back(v0.z);
    verts.push_back(n0.x); verts.push_back(n0.y); verts.push_back(n0.z);
    verts.push_back(t0.x); verts.push_back(t0.y);
    verts.push_back(v3.x); verts.push_back(v3.y); verts.push_back(v3.z);
    verts.push_back(n3.x); verts.push_back(n3.y); verts.push_back(n3.z);
    verts.push_back(t3.x); verts.push_back(t3.y);
    verts.push_back(v1.x); verts.push_back(v1.y); verts.push_back(v1.z);
    verts.push_back(n1.x); verts.push_back(n1.y); verts.push_back(n1.z);
    verts.push_back(t1.x); verts.push_back(t1.y);

    verts.push_back(v0.x); verts.push_back(v0.y); verts.push_back(v0.z);
    verts.push_back(n0.x); verts.push_back(n0.y); verts.push_back(n0.z);
    verts.push_back(t0.x); verts.push_back(t0.y);
    verts.push_back(v2.x); verts.push_back(v1.y); verts.push_back(v1.z);
    verts.push_back(n2.x); verts.push_back(n1.y); verts.push_back(n1.z);
    verts.push_back(t2.x); verts.push_back(t1.y);
    verts.push_back(v3.x); verts.push_back(v3.y); verts.push_back(v3.z);
    verts.push_back(n3.x); verts.push_back(n3.y); verts.push_back(n3.z);
    verts.push_back(t3.x); verts.push_back(t3.y);

    verts.push_back(v1.x); verts.push_back(v1.y); verts.push_back(v1.z);
    verts.push_back(n1.x); verts.push_back(n1.y); verts.push_back(n1.z);
    verts.push_back(t1.x); verts.push_back(t1.y);
    verts.push_back(v3.x); verts.push_back(v3.y); verts.push_back(v3.z);
    verts.push_back(n3.x); verts.push_back(n3.y); verts.push_back(n3.z);
    verts.push_back(t3.x); verts.push_back(t3.y);
    verts.push_back(v2.x); verts.push_back(v1.y); verts.push_back(v1.z);
    verts.push_back(n2.x); verts.push_back(n1.y); verts.push_back(n1.z);
    verts.push_back(t2.x); verts.push_back(t1.y);

    graphics->addShape("tetrahedron", verts);


    addScreen("menu", std::make_shared<FinalMenuScreen>());
    addScreen("debug", std::make_shared<FinalDebugScreen>());
    setScreen("menu");
}
