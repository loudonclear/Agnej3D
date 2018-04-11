#include "Screen.h"
#include "graphics/Graphics.h"

Screen::Screen()
{
}

void Screen::setApplication(Application* app) {
    application = app;
}

Application* Screen::getApplication() {
    return application;
}
