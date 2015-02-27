#pragma once

#include <QtCore>
#include "vector.h"
#include "triangle.h"

class ObjectModel {
private:
    ObjectModel() {}
public:
    static ObjectModel *fromWareFrontObjectFile(QString file_path);

    QVector<Vec4d> const &getVertexList() const { return vertexList; }
    QVector<Triangle> const &getTrianglesList() const { return trianglesList; }

private:
    QVector<Vec4d> vertexList;
    QVector<Triangle> trianglesList;
};