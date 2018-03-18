#include "obj.h"

#include <QFile>
#include <QTextStream>
#include <iostream>

OBJ::OBJ(QString path) : vertexCount(0)
{
    read(path);
}

OBJ::~OBJ()
{
    for (int i = 0; i < triangles.size(); i++) {
        delete triangles.at(i);
    }
}

bool OBJ::read(const QString &path)
{
    // Open the file
    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        std::cout << "ERROR: File could not be read." << std::endl;
        return false;
    }

    QTextStream f(&file);
    QString line;

    // Read the file
    QRegExp spaces("\\s+");
    do {
        line = f.readLine().trimmed();
        QStringList parts = line.split(spaces);
        if (parts.isEmpty()) continue;

        if (parts[0] == "v" && parts.count() >= 4) {
            vertices += glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
        } else if (parts[0] == "vt" && parts.count() >= 3) {
            coords += glm::vec2(parts[1].toFloat(), parts[2].toFloat());
        } else if (parts[0] == "vn" && parts.count() >= 4) {
            normals += glm::vec3(parts[1].toFloat(), parts[2].toFloat(), parts[3].toFloat());
        } else if (parts[0] == "f" && parts.count() >= 4) {
            // Convert polygons into triangle fans
            Index a = getIndex(parts[1]);
            Index b = getIndex(parts[2]);
            for (int i = 3; i < parts.count(); i++) {
                Index c = getIndex(parts[i]);

                Triangle* curr;
                if (normals.size() > 0)
                    curr = new Triangle(a, b, c, vertices.at(a.vertexIndex), vertices.at(b.vertexIndex), vertices.at(c.vertexIndex), normals.at(a.normalIndex));
                else
                    curr = new Triangle(a, b, c, vertices.at(a.vertexIndex), vertices.at(b.vertexIndex), vertices.at(c.vertexIndex));

                triangles.append(curr);
                addTriangleFloats(curr);

                b = c;
            }
        }
    } while (!line.isNull());

    return true;
}

void OBJ::addTriangleFloats(Triangle *tri)
{
    for (int i = 0; i < 3; i++) {
        vboData.append(tri->vertices[i].x);
        vboData.append(tri->vertices[i].y);
        vboData.append(tri->vertices[i].z);
        vboData.append(tri->normal.x);
        vboData.append(tri->normal.y);
        vboData.append(tri->normal.z);

        Index curr = i == 0 ? tri->a : i == 1 ? tri->b : tri->c;
        if (coords.size() > 0) {
            if (curr.coordIndex >= 0 && curr.coordIndex < coords.size()) {
                vboData.append(coords.at(curr.coordIndex).x);
                vboData.append(coords.at(curr.coordIndex).y);
            } else {
                vboData.append(0);
                vboData.append(0);
            }
        } else {
            vboData.append(0);
            vboData.append(0);
        }

        vertexCount += 1;
    }
}

inline int relativeIndex(int index, int count)
{
    return index >= 0 ? index - 1 : count + index;
}

OBJ::Index OBJ::getIndex(const QString &str) const
{
    QStringList parts = str.split('/');
    int vertex = parts.count() > 0 ? relativeIndex(parts[0].toInt(), vertices.count()) : -1;
    int coord = parts.count() > 1 ? relativeIndex(parts[1].toInt(), coords.count()) : -1;
    int normal = parts.count() > 2 ? relativeIndex(parts[2].toInt(), normals.count()) : -1;
    return Index(vertex, coord, normal);
}

static QString str(const glm::vec2 &v) { return QString("%1 %2").arg(v.x).arg(v.y); }
static QString str(const glm::vec3 &v) { return QString("%1 %2 %3").arg(v.x).arg(v.y).arg(v.z); }

static QString str(const OBJ::Index &i)
{
    if (i.normalIndex >= 0) {
        if (i.coordIndex >= 0) return QString("%1/%2/%3").arg(i.vertexIndex + 1).arg(i.coordIndex + 1).arg(i.normalIndex + 1);
        return QString("%1//%2").arg(i.vertexIndex + 1).arg(i.normalIndex + 1);
    } else {
        if (i.coordIndex >= 0) return QString("%1/%2").arg(i.vertexIndex + 1).arg(i.coordIndex + 1);
        return QString("%1").arg(i.vertexIndex + 1);
    }
}

bool OBJ::write(const QString &path) const
{
    // Open the file
    QFile file(path);
    if (!file.open(QFile::WriteOnly | QFile::Text)) return false;
    QTextStream f(&file);

    // Write the file
    foreach (const glm::vec3 &vertex, vertices) f << "v " << str(vertex) << '\n';
    foreach (const glm::vec2 &coord, coords) f << "vt " << str(coord) << '\n';
    foreach (const glm::vec3 &normal, normals) f << "vn " << str(normal) << '\n';

    foreach (const Triangle *tri, triangles) f << "f " << str(tri->a) << ' ' << str(tri->b) << ' ' << str(tri->c) << '\n';

    return true;
}
