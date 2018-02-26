#include "Chunk.h"
#include "engine/world/GameObject.h"
#include "engine/components/GraphicsShapeComponent.h"
#include "engine/graphics/Shape.h"


Chunk::Chunk(GameObject *parent, const int dimx, const int dimy, const int dimz) : Component(parent), dimx(dimx), dimy(dimy), dimz(dimz)
{
    m_blocks.resize(dimx * dimy * dimz);
}



void Chunk::init() {
    gc = std::dynamic_pointer_cast<GraphicsShapeComponent>(m_gameObject->getComponent<GraphicsComponent>());
    update();
}

Block* Chunk::getBlock(int x, int y, int z) {
    if (x < 0 || x >= dimx || y < 0 || y >= dimy || z < 0 || z >= dimz) return nullptr;

    const unsigned int index = x*dimy*dimz + y*dimz + z;
    assert(index >= 0 && index < m_blocks.size());
    return m_blocks[index];
}

void Chunk::setBlock(int x, int y, int z, Block& block) {
    if (x < 0 || x >= dimx || y < 0 || y >= dimy || z < 0 || z >= dimz) return;

    const int index = x*dimy*dimz + y*dimz + z;
    assert(index >= 0 && index < m_blocks.size());
    assert(&block != nullptr);
    m_blocks[index] = &block;
}

void Chunk::update() {
    std::vector<float> data;
    std::vector<int> triangles;
    data.reserve(dimx * dimy * dimz * 24 * 8);
    triangles.reserve(dimx * dimy * dimz * 12 * 3);
    int numVertices = 0;

    for (int x = 0; x < dimx; x++) {
        for (int y = 0; y < dimy; y++) {
            for (int z = 0; z < dimz; z++) {
                const unsigned int index = x*dimy*dimz + y*dimz + z;
                Block* b = m_blocks[index];

                if (!b->transparent) {
                    Block *left = getBlock(x - 1, y, z);
                    Block *right = getBlock(x + 1, y, z);
                    Block *down = getBlock(x, y - 1, z);
                    Block *up = getBlock(x, y + 1, z);
                    Block *back = getBlock(x, y, z - 1);
                    Block *front = getBlock(x, y, z + 1);


                    if (left == nullptr || left->transparent) {
                        data.push_back(x); data.push_back(y); data.push_back(z);
                        data.push_back(-1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[0]); data.push_back(b->texcoords[1]);

                        data.push_back(x); data.push_back(y + 1); data.push_back(z);
                        data.push_back(-1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[2]); data.push_back(b->texcoords[3]);

                        data.push_back(x); data.push_back(y); data.push_back(z + 1);
                        data.push_back(-1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[4]); data.push_back(b->texcoords[5]);

                        data.push_back(x); data.push_back(y + 1); data.push_back(z + 1);
                        data.push_back(-1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[6]); data.push_back(b->texcoords[7]);

                        triangles.push_back(numVertices); triangles.push_back(numVertices + 2); triangles.push_back(numVertices + 3);
                        triangles.push_back(numVertices); triangles.push_back(numVertices + 3); triangles.push_back(numVertices + 1);
                        numVertices += 4;
                    }

                    if (right == nullptr || right->transparent) {
                        data.push_back(x + 1); data.push_back(y); data.push_back(z);
                        data.push_back(1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[8]); data.push_back(b->texcoords[9]);

                        data.push_back(x + 1); data.push_back(y + 1); data.push_back(z);
                        data.push_back(1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[10]); data.push_back(b->texcoords[11]);

                        data.push_back(x + 1); data.push_back(y); data.push_back(z + 1);
                        data.push_back(1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[12]); data.push_back(b->texcoords[13]);

                        data.push_back(x + 1); data.push_back(y + 1); data.push_back(z + 1);
                        data.push_back(1); data.push_back(0); data.push_back(0);
                        data.push_back(b->texcoords[14]); data.push_back(b->texcoords[15]);

                        triangles.push_back(numVertices + 2); triangles.push_back(numVertices); triangles.push_back(numVertices + 1);
                        triangles.push_back(numVertices + 2); triangles.push_back(numVertices + 1); triangles.push_back(numVertices + 3);
                        numVertices += 4;
                    }

                    if (down == nullptr || down->transparent) {
                        data.push_back(x); data.push_back(y); data.push_back(z);
                        data.push_back(0); data.push_back(-1); data.push_back(0);
                        data.push_back(b->texcoords[16]); data.push_back(b->texcoords[17]);

                        data.push_back(x + 1); data.push_back(y); data.push_back(z);
                        data.push_back(0); data.push_back(-1); data.push_back(0);
                        data.push_back(b->texcoords[18]); data.push_back(b->texcoords[19]);

                        data.push_back(x); data.push_back(y); data.push_back(z + 1);
                        data.push_back(0); data.push_back(-1); data.push_back(0);
                        data.push_back(b->texcoords[20]); data.push_back(b->texcoords[21]);

                        data.push_back(x + 1); data.push_back(y); data.push_back(z + 1);
                        data.push_back(0); data.push_back(-1); data.push_back(0);
                        data.push_back(b->texcoords[22]); data.push_back(b->texcoords[23]);

                        triangles.push_back(numVertices); triangles.push_back(numVertices + 1); triangles.push_back(numVertices + 3);
                        triangles.push_back(numVertices); triangles.push_back(numVertices + 3); triangles.push_back(numVertices + 2);
                        numVertices += 4;
                    }

                    if (up == nullptr || up->transparent) {
                        data.push_back(x); data.push_back(y + 1); data.push_back(z);
                        data.push_back(0); data.push_back(1); data.push_back(0);
                        data.push_back(b->texcoords[24]); data.push_back(b->texcoords[25]);

                        data.push_back(x + 1); data.push_back(y + 1); data.push_back(z);
                        data.push_back(0); data.push_back(1); data.push_back(0);
                        data.push_back(b->texcoords[26]); data.push_back(b->texcoords[27]);

                        data.push_back(x); data.push_back(y + 1); data.push_back(z + 1);
                        data.push_back(0); data.push_back(1); data.push_back(0);
                        data.push_back(b->texcoords[28]); data.push_back(b->texcoords[29]);

                        data.push_back(x + 1); data.push_back(y + 1); data.push_back(z + 1);
                        data.push_back(0); data.push_back(1); data.push_back(0);
                        data.push_back(b->texcoords[30]); data.push_back(b->texcoords[31]);

                        triangles.push_back(numVertices + 2); triangles.push_back(numVertices + 3); triangles.push_back(numVertices + 1);
                        triangles.push_back(numVertices + 2); triangles.push_back(numVertices + 1); triangles.push_back(numVertices);
                        numVertices += 4;
                    }

                    if (back == nullptr || back->transparent) {
                        data.push_back(x); data.push_back(y); data.push_back(z);
                        data.push_back(0); data.push_back(0); data.push_back(-1);
                        data.push_back(b->texcoords[32]); data.push_back(b->texcoords[33]);

                        data.push_back(x + 1); data.push_back(y); data.push_back(z);
                        data.push_back(0); data.push_back(0); data.push_back(-1);
                        data.push_back(b->texcoords[34]); data.push_back(b->texcoords[35]);

                        data.push_back(x); data.push_back(y + 1); data.push_back(z);
                        data.push_back(0); data.push_back(0); data.push_back(-1);
                        data.push_back(b->texcoords[36]); data.push_back(b->texcoords[37]);

                        data.push_back(x + 1); data.push_back(y + 1); data.push_back(z);
                        data.push_back(0); data.push_back(0); data.push_back(-1);
                        data.push_back(b->texcoords[38]); data.push_back(b->texcoords[39]);

                        triangles.push_back(numVertices + 1); triangles.push_back(numVertices); triangles.push_back(numVertices + 2);
                        triangles.push_back(numVertices + 1); triangles.push_back(numVertices + 2); triangles.push_back(numVertices + 3);
                        numVertices += 4;
                    }

                    if (front == nullptr || front->transparent) {
                        data.push_back(x); data.push_back(y); data.push_back(z + 1);
                        data.push_back(0); data.push_back(0); data.push_back(1);
                        data.push_back(b->texcoords[40]); data.push_back(b->texcoords[41]);

                        data.push_back(x + 1); data.push_back(y); data.push_back(z + 1);
                        data.push_back(0); data.push_back(0); data.push_back(1);
                        data.push_back(b->texcoords[42]); data.push_back(b->texcoords[43]);

                        data.push_back(x); data.push_back(y + 1); data.push_back(z + 1);
                        data.push_back(0); data.push_back(0); data.push_back(1);
                        data.push_back(b->texcoords[44]); data.push_back(b->texcoords[45]);

                        data.push_back(x + 1); data.push_back(y + 1); data.push_back(z + 1);
                        data.push_back(0); data.push_back(0); data.push_back(1);
                        data.push_back(b->texcoords[46]); data.push_back(b->texcoords[47]);

                        triangles.push_back(numVertices); triangles.push_back(numVertices + 1); triangles.push_back(numVertices + 3);
                        triangles.push_back(numVertices); triangles.push_back(numVertices + 3); triangles.push_back(numVertices + 2);
                        numVertices += 4;
                    }


                }


            }
        }
    }

    gc->setShape(data, triangles);
}

