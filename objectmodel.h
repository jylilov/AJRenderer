#pragma once

#include <QtCore>
#include "vector.h"
#include "triangle.h"
#include "texture.h"

class ObjectModel {
private:
    ObjectModel() {}
public:
    static ObjectModel *fromWareFrontObjectFile(QString file_path);

    QVector<Vec4d> const &getVertexList() const { return vertexList; }
    QVector<Triangle> const &getTrianglesList() const { return trianglesList; }

    void setTexture(Texture *texture = 0) { this->texture = texture; }
    Texture *getTexture() { return texture; }

private:
    QVector<Vec4d> vertexList;
    QVector<Triangle> trianglesList;
    QVector<Vec3d> textureVertexList;

    Texture *texture;
};