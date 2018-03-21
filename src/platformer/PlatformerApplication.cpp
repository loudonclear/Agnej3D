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

    graphics->addTexture("easy", ":/platformer/level_easy.png");
    Material levelTex;
    levelTex.textureName = "easy";
    levelTex.textureRepeat = glm::vec2(1, 1);
    graphics->addMaterial("easy", levelTex);

    std::shared_ptr<OBJ> obj = std::make_shared<OBJ>(":/platformer/level_easy.obj");
    graphics->addOBJ("easy", obj);
    graphics->addShape("easy", obj->vboData.toStdVector());

    graphics->addTexture("island", ":/platformer/level_island.png");
    Material levelTex2;
    levelTex2.textureName = "island";
    levelTex2.textureRepeat = glm::vec2(1, 1);
    graphics->addMaterial("island", levelTex2);

    std::shared_ptr<OBJ> obj2 = std::make_shared<OBJ>(":/platformer/level_island.obj");
    graphics->addOBJ("island", obj2);
    graphics->addShape("island", obj2->vboData.toStdVector());


    addScreen("menu", std::make_shared<PlatformerMenuScreen>());

    setScreen("menu");
}
