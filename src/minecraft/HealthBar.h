#ifndef HEALTHBAR_H
#define HEALTHBAR_H


#include "engine/components/HUDComponent.h"

class HealthBar : public virtual HUDComponent
{
public:
    HealthBar(GameObject *parent, const std::string &materialName, glm::vec3 offset, glm::vec3 scale);

    void draw(Graphics *g);

    void setHealth(float health);
    float getHealth();

private:
    glm::vec3 m_offset, m_scale;

    float m_health;
};

#endif // HEALTHBAR_H
