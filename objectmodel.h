#pragma once

#include <QtCore>
#include "vector.h"
#include "triangle.h"
#include "texture.h"
#include "matrix.h"

class ObjectModel {
private:
    ObjectModel() : size(1), direction() { updateMatrix(); }
    void updateMatrix();
public:
    static ObjectModel *fromWareFrontObjectFile(QString file_path);

    QVector<Triangle> const &getTrianglesList() const { return trianglesList; }

    void setTexture(Texture *texture = 0) { this->texture = texture; }
    Texture *getTexture() { return texture; }

    double getSize() const { return size; }
    void setSize(double size) { ObjectModel::size = size; updateMatrix(); }
    Vec3d getDirection() const { return direction; }
    void setDirection(Vec3d direction) { ObjectModel::direction = direction; updateMatrix(); }
    Mat4d getModelMatrix() const { return modelMatrix; }

private:
    double size;
    Vec3d direction;
    Mat4d modelMatrix;

    QVector<Vec4d> vertexList;
    QVector<Triangle> trianglesList;
    QVector<Vec3d> textureVertexList;

    Texture *texture;
};