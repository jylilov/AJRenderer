#pragma once

#include <QtCore>
#include "vector.h"

class WarefrontObject {
public:
    WarefrontObject(QString file_path);
    QVector<Vec4d> const &getVertexList() const { return vertexList; }
    QVector<Vec3i> const &getTrianglesList() const { return trianglesList; }

    uint getSize() const { return size; }
    void setSize(uint size) { WarefrontObject::size = size; }
    Vec4d const &getCoordinates() const { return coordinates; }
    void setCoordinates(Vec4d &coordinates) { WarefrontObject::coordinates = coordinates; }

private:
    uint size;
    Vec4d coordinates;
    QVector<Vec4d> vertexList;
    QVector<Vec3i> trianglesList;
};