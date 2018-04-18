#ifndef GRAPHICSSHAPECOMPONENT_H
#define GRAPHICSSHAPECOMPONENT_H

#include "GraphicsComponent.h"
#include "engine/graphics/Shape.h"

class ShapeCollider;

class GraphicsShapeComponent : public virtual GraphicsComponent
{
public:
    GraphicsShapeComponent(GameObject *parent, const std::string &shapeName, const std::string &materialName = "default", bool frustumCull = false);
    GraphicsShapeComponent(GameObject *parent, std::shared_ptr<Shape> shape, const std::string &materialName = "default", bool frustumCull = false);
    void draw(Graphics *g);
    void init();

    void setShape(std::vector<float> &data, std::vector<int> &triangles) {
        m_shape = std::make_shared<Shape>(data, triangles);
        m_shape->build();
    }

private:
    std::shared_ptr<Shape> m_shape;
    std::shared_ptr<ShapeCollider> m_shapeCollider;

    bool m_frustumCull;
};

#endif // GRAPHICSSHAPECOMPONENT_H
