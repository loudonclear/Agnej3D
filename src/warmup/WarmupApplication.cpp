#include "WarmupApplication.h"

#include "WarmupMenuScreen.h"
#include "WarmupGameScreen.h"
#include "WarmupDebugScreen.h"
#include "engine/graphics/Material.h"
#include "engine/graphics/Graphics.h"

WarmupApplication::WarmupApplication()
{
    addScreen("menu", std::make_shared<WarmupMenuScreen>());
    addScreen("game", std::make_shared<WarmupGameScreen>());
    addScreen("debug", std::make_shared<WarmupDebugScreen>());
    setScreen("menu");

    Graphics *graphics = Graphics::getGlobalInstance();

    Material grass;
    grass.textureName = "grass";
    grass.textureRepeat = glm::vec2(10, 10);
    graphics->addMaterial("grass", grass);
}
