#ifndef GRAPHICSSHAPECOMPONENT_H
#define GRAPHICSSHAPECOMPONENT_H

#include "GraphicsComponent.h"

class ShapeCollider;

class GraphicsShapeComponent : public GraphicsComponent
{
public:
    GraphicsShapeComponent(std::shared_ptr<GameObject> &parent, const std::string &shapeName, const std::string &materialName = "default");
    GraphicsShapeComponent(std::shared_ptr<GameObject> &parent, std::shared_ptr<Shape> shape, const std::string &materialName = "default");
    void draw(Graphics *g);

    void setShape(std::shared_ptr<Shape> shape) { m_shape = shape; }

private:
    std::shared_ptr<Shape> m_shape;
};

#endif // GRAPHICSSHAPECOMPONENT_H
