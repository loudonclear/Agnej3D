#include "WarmupApplication.h"

#include "WarmupMenuScreen.h"
#include "WarmupGameScreen.h"

WarmupApplication::WarmupApplication()
{
    addScreen("menu", std::make_shared<WarmupMenuScreen>());
    addScreen("game", std::make_shared<WarmupGameScreen>());
    setScreen("menu");
}
