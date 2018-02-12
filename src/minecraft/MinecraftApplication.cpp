#include "MinecraftApplication.h"

#include "MinecraftMenuScreen.h"
#include "MinecraftGameScreen.h"

#include "engine/graphics/Material.h"
#include "engine/graphics/Graphics.h"

MinecraftApplication::MinecraftApplication()
{
    addScreen("menu", std::make_shared<MinecraftMenuScreen>());
    addScreen("game", std::make_shared<MinecraftGameScreen>());
    setScreen("menu");

    Graphics *graphics = Graphics::getGlobalInstance();

    graphics->addTexture("terrain", ":/images/terrain.png");

    Material grass;
    grass.textureName = "grass";
    grass.textureRepeat = glm::vec2(10, 10);
    graphics->addMaterial("grass", grass);

    Material textureAtlas;
    textureAtlas.textureName = "terrain";
    textureAtlas.textureRepeat = glm::vec2(1, 1);
    graphics->addMaterial("terrain", textureAtlas);
}
