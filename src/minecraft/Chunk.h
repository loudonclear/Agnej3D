#ifndef CHUNK_H
#define CHUNK_H


#include <map>
#include <vector>
#include "glm/glm.hpp"

#include "engine/components/Component.h"

#include <iostream>

class GraphicsShapeComponent;

struct Block {

    Block(bool trans, bool pass, float offset, glm::vec2 left, glm::vec2 right, glm::vec2 down, glm::vec2 up, glm::vec2 back, glm::vec2 front) : transparent(trans), passable(pass)
    {
        texcoords.reserve(48);
        texcoords.push_back(left.x * offset);                texcoords.push_back(left.y * offset);
        texcoords.push_back(left.x * offset);                texcoords.push_back(left.y * offset + offset);
        texcoords.push_back(left.x * offset + offset);       texcoords.push_back(left.y * offset);
        texcoords.push_back(left.x * offset + offset);       texcoords.push_back(left.y * offset + offset);

        texcoords.push_back(right.x * offset);                texcoords.push_back(right.y * offset);
        texcoords.push_back(right.x * offset);                texcoords.push_back(right.y * offset + offset);
        texcoords.push_back(right.x * offset + offset);       texcoords.push_back(right.y * offset);
        texcoords.push_back(right.x * offset + offset);       texcoords.push_back(right.y * offset + offset);

        texcoords.push_back(down.x * offset);                texcoords.push_back(down.y * offset + offset);
        texcoords.push_back(down.x * offset);                texcoords.push_back(down.y * offset);
        texcoords.push_back(down.x * offset + offset);       texcoords.push_back(down.y * offset + offset);
        texcoords.push_back(down.x * offset + offset);       texcoords.push_back(down.y * offset);

        texcoords.push_back(up.x * offset);                texcoords.push_back(up.y * offset);
        texcoords.push_back(up.x * offset);                texcoords.push_back(up.y * offset + offset);
        texcoords.push_back(up.x * offset + offset);       texcoords.push_back(up.y * offset);
        texcoords.push_back(up.x * offset + offset);       texcoords.push_back(up.y * offset + offset);

        texcoords.push_back(back.x * offset + offset);      texcoords.push_back(back.y * offset);
        texcoords.push_back(back.x * offset);               texcoords.push_back(back.y * offset);
        texcoords.push_back(back.x * offset + offset);      texcoords.push_back(back.y * offset + offset);
        texcoords.push_back(back.x * offset);               texcoords.push_back(back.y * offset + offset);

        texcoords.push_back(front.x * offset);                texcoords.push_back(front.y * offset);
        texcoords.push_back(front.x * offset + offset);       texcoords.push_back(front.y * offset);
        texcoords.push_back(front.x * offset);                texcoords.push_back(front.y * offset + offset);
        texcoords.push_back(front.x * offset + offset);       texcoords.push_back(front.y * offset + offset);
    }

    bool transparent;
    bool passable;
    std::vector<float> texcoords;

};


class Chunk : public Component
{
public:
    Chunk(GameObject *parent, const int dimx, const int dimy, const int dimz);

    void init();
    void update();

    Block* getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, Block &block);
private:

    std::shared_ptr<GraphicsShapeComponent> gc;

    std::vector<Block*> m_blocks;
    const int dimx, dimy, dimz;
};

#endif // CHUNK_H
