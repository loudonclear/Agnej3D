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

    addScreen("menu", std::make_shared<PlatformerMenuScreen>());

    setScreen("menu");
}
