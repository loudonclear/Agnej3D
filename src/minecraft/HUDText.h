#ifndef HUDTEXT_H
#define HUDTEXT_H


#include "engine/components/HUDComponent.h"

class HUDText : public virtual HUDComponent
{
public:
    HUDText(GameObject *parent, const std::string &materialName, glm::vec3 offset, float scale, std::string text);

    void draw(Graphics *g);

private:
    glm::vec3 m_offset;
    float m_scale;

    std::string m_text;
};

#endif // HUDTEXT_H
