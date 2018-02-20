#ifndef VISUALCOLLIDE_H
#define VISUALCOLLIDE_H

#include "engine/components/TickComponent.h"

class GraphicsComponent;
class ShapeCollider;

class VisualCollide : public TickComponent
{
public:
    VisualCollide(GameObject *parent);

    void tick(float seconds);
    void init();

private:
    std::shared_ptr<GraphicsComponent> m_gc;
    std::shared_ptr<ShapeCollider> m_sc;
};

#endif // VISUALCOLLIDE_H
