#include "PlatformerApplication.h"

#include "PlatformerMenuScreen.h"
#include "PlatformerScreen.h"

#include "engine/graphics/Material.h"
#include "engine/graphics/Graphics.h"
#include "engine/graphics/Shape.h"
#include "engine/graphics/Obj.h"

PlatformerApplication::PlatformerApplication()
{
    Graphics *graphics = Graphics::getGlobalInstance();

    Material button;
    button.color = glm::vec3(0.1f, 0.1f, 0.1f);
    graphics->addMaterial("button", button);

    Material nav;
    nav.color = glm::vec3(0.68f, 0.85f, 0.9f);
    graphics->addMaterial("nav", nav);

    graphics->addTexture("easy", ":/platformer/level_easy.png");
    Material levelTex;
    levelTex.textureName = "easy";
    levelTex.textureRepeat = glm::vec2(1, 1);
    graphics->addMaterial("easy", levelTex);

    std::shared_ptr<OBJ> easy = std::make_shared<OBJ>(":/platformer/level_easy.obj");
    graphics->addOBJ("easy", easy);
    graphics->addShape("easy", easy->vboData.toStdVector());

    std::shared_ptr<OBJ> easyNav = std::make_shared<OBJ>(":/platformer/level_easy_navmesh.obj");
    graphics->addOBJ("easyNav", easyNav);
    graphics->addShape("easyNav", easyNav->vboData.toStdVector());

    graphics->addTexture("island", ":/platformer/level_island.png");
    Material levelTex2;
    levelTex2.textureName = "island";
    levelTex2.textureRepeat = glm::vec2(1, 1);
    graphics->addMaterial("island", levelTex2);

    std::shared_ptr<OBJ> island = std::make_shared<OBJ>(":/platformer/level_island.obj");
    graphics->addOBJ("island", island);
    graphics->addShape("island", island->vboData.toStdVector());

    std::shared_ptr<OBJ> islandNav = std::make_shared<OBJ>(":/platformer/level_island_navmesh.obj");
    graphics->addOBJ("islandNav", islandNav);
    graphics->addShape("islandNav", islandNav->vboData.toStdVector());


    addScreen("menu", std::make_shared<PlatformerMenuScreen>());

    setScreen("menu");
}
