#ifndef NPCTEXT_H
#define NPCTEXT_H

#include "engine/components/OrthographicUIComponent.h"

class NPCText : public OrthographicUIComponent
{
public:
    NPCText(GameObject *parent, const std::string &materialName, glm::vec3 offset, std::string text = "");

    void draw(Graphics *g);

    void setText(std::string text);

private:
    glm::vec3 m_offset;
    std::string m_text;
};

#endif // NPCTEXT_H
