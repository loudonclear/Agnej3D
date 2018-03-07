#ifndef GRAPHICSSYSTEM_H
#define GRAPHICSSYSTEM_H

#include "System.h"

class GraphicsComponent;
class Graphics;
class UIComponent;

class GraphicsSystem : public System
{
public:
    GraphicsSystem();

    void addGameObject(const std::shared_ptr<GameObject> &go);
    void removeGameObject(const std::shared_ptr<GameObject> &go);

    void draw(Graphics *g);

private:

    std::vector<std::shared_ptr<GraphicsComponent>> m_components;
    std::vector<std::shared_ptr<UIComponent>> m_uiComponents;
};

#endif // GRAPHICSSYSTEM_H
