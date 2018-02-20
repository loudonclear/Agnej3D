#ifndef GRAPHICSCOMPONENT_H
#define GRAPHICSCOMPONENT_H


#include <string>
#include <memory>
#include "Component.h"
#include "engine/graphics/Graphics.h"

class Transform;

class GraphicsComponent : public Component
{
public:
    GraphicsComponent(GameObject *parent, const std::string &materialName = "default");

    virtual void init();
    virtual void draw(Graphics *g) = 0;

    void setMaterial(const std::string &materialName) { m_materialName = materialName; }

protected:
    std::string m_materialName;
    std::shared_ptr<Transform> m_transform;
};

#endif // GRAPHICSCOMPONENT_H
