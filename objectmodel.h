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

    static QString getFileName(QFile const &file);
    static void loadTextures(ObjectModel *object, QFile &file);
public:
    static ObjectModel *fromWareFrontObjectFile(QString file_path);
private:
    QString name;

    Vec3d size;
    Vec3d direction;
    Vec3d position;

    Mat4d modelMatrix;

    QVector<Vec3d> vertexList;
    QVector<Vec2d> textureVertexList;
    QVector<Vec3d> normalVectors;

    QVector<Vector<3, Vertex> > triangles;

    Texture *diffuseTexture;
    Texture *specTexture;
    Texture *nmTexture;
public:
    ~ObjectModel();
    Texture *getDiffuseTexture() const { return diffuseTexture; }
    Texture *getNmTexture() const { return nmTexture; }
    Texture *getSpecTexture() const { return specTexture; }

    Vec3d getSize() const { return size; }
    void setSize(Vec3d const &size) { this->size = size; updateMatrix(); }
    Vec3d getDirection() const { return direction; }
    void setDirection(Vec3d const &direction) { this->direction = direction; updateMatrix(); }
    Vec3d getPosition() const { return position; }
    void setPosition(Vec3d const &position) { ObjectModel::position = position; updateMatrix();  }
    QString getName() const { return name; }
    void setName(QString const &name) { ObjectModel::name = name; }
    Mat4d getModelMatrix() const { return modelMatrix; }
    QVector<Vector<3, Vertex> > const &getTriangles() const { return triangles; }
private:
    ObjectModel() : size(1.0, 1.0, 1.0), direction() { updateMatrix(); }

    void updateMatrix();
};