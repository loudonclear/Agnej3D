#ifndef OBJ_H
#define OBJ_H

#include <QVector>
#include <QHash>
#include <QString>
#include <QPair>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

/* A simple parser that reads and writes Wavefront .obj files.
 *
 * Example usage:
 *  OBJ *easy = new OBJ("easy.obj");    // reads the file "easy.obj" into memory
 *  easy->vboData;                      // a QVector containing the float data in interleaved format
 *  easy->vertexCount;                  // the number of vertices in the data
 *  easy->triangles;                    // a QList containing a number of Triangle structs, to use in your collisions
 */

class OBJ
{
public:
    // Used for reading/writing OBJ files. Don't worry too much about using or understanding this struct.
    struct Index
    {
        int vertexIndex;
        int coordIndex;
        int normalIndex;

        Index() : vertexIndex(-1), coordIndex(-1), normalIndex(-1) {}
        Index(int vertexIndex, int coordIndex, int normalIndex) : vertexIndex(vertexIndex), coordIndex(coordIndex), normalIndex(normalIndex) {}
    };

    // A basic struct that holds all the basic information about a single triangle in an OBJ model.
    struct Triangle
    {
        Index a, b, c;
        glm::vec3 vertices[3];
        glm::vec3 normal;

        Triangle(const Index &inda, const Index &indb, const Index &indc,
                 const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3) {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;
            normal = glm::normalize(glm::cross(vertices[1] - vertices[0], vertices[2] - vertices[0]));

            a = inda;
            b = indb;
            c = indc;
        }

        Triangle(const Index &inda, const Index &indb, const Index &indc,
                 const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &currNormal) {
            vertices[0] = v1;
            vertices[1] = v2;
            vertices[2] = v3;

            normal = glm::normalize(currNormal);

            a = inda;
            b = indb;
            c = indc;
        }
    };

    // The constructor automatically reads the given file.
    OBJ(QString path);
    ~OBJ();

    // 1D arrays indexed by the Index structs. You shouldn't need to use these.
    QVector<glm::vec3> vertices;
    QVector<glm::vec2> coords;
    QVector<glm::vec3> normals;

    /* Use these to construct your VBO's. The VBO data is formatted like our Cylinder and Ellipsoid headers:
     * interleaved vertex, normal, and tex coords. */
    QVector<float> vboData;
    int vertexCount;

    // The triangles that make up the VBO.
    QList<Triangle*> triangles;

    bool read(const QString &path);
    bool write(const QString &path) const;

private:
    Index getIndex(const QString &str) const;
    void addTriangleFloats(Triangle *tri);
};

#endif // OBJ_H

