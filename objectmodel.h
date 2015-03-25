#pragma once

#include <QtCore>
#include "vector.h"
#include "texture.h"
#include "matrix.h"
#include "vertex.h"

class ObjectModel {
private:
    static Vec3d parseVector(QList<QString>::const_iterator &iterator);
    static Vec2d parseTextureVector(QList<QString>::const_iterator &iterator);
    static Vector<3, Vec3i> parseTriangle(QList<QString>::const_iterator &iterator);
    static void addTriangles(ObjectModel *object, QList< Vector<3, Vec3i> > &triangles);

public:
    static ObjectModel *fromWareFrontObjectFile(QString file_path);

private:
    ObjectModel() : size(1), direction() { updateMatrix(); }
    void updateMatrix();

public:
    void setDiffuseTexture(Texture *texture = 0) { this->diffuseTexture = texture; }
    Texture *getDiffuseTexture() { return diffuseTexture; }
    void setNmTexture(Texture *nmTexture) { ObjectModel::nmTexture = nmTexture; }
    Texture *getNmTexture() const { return nmTexture; }
    Texture *getSpecTexture() const { return specTexture; }

    void setSpecTexture(Texture *specTexture) { ObjectModel::specTexture = specTexture; }
    double getSize() const { return size; }
    void setSize(double size) { ObjectModel::size = size; updateMatrix(); }
    Vec3d getDirection() const { return direction; }
    void setDirection(Vec3d direction) { ObjectModel::direction = direction; updateMatrix(); }

    Mat4d getModelMatrix() const { return modelMatrix; }

    QVector<Vector<3, Vertex> > const &getTriangles() const { return triangles; }

private:
    double size;
    Vec3d direction;

    Mat4d modelMatrix;

    QVector<Vec3d> vertexList;
    QVector<Vec2d> textureVertexList;
    QVector<Vec3d> normalVectors;

    QVector<Vector<3, Vertex> > triangles;

    Texture *diffuseTexture;
    Texture *specTexture;
    Texture *nmTexture;
};