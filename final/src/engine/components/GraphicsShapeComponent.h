#ifndef GRAPHICSSHAPECOMPONENT_H
#define GRAPHICSSHAPECOMPONENT_H

#include "GraphicsComponent.h"

class ShapeCollider;

class GraphicsShapeComponent : public GraphicsComponent
{
public:
    GraphicsShapeComponent(std::shared_ptr<GameObject> &parent, const std::string &shapeName, const std::string &materialName = "default");
    void draw(Graphics *g);

    void setShape(const std::string &shapeName) { m_shapeName = shapeName; }

private:
    std::string m_shapeName;
};

#endif // GRAPHICSSHAPECOMPONENT_H
