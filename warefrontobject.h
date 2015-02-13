#pragma once

#include <QtCore>
#include "vector.h"

class WarefrontObject : QObject {
    Q_OBJECT
public:
    WarefrontObject(QString file_path, QObject *parent = 0);
    QVector<Vec3d> const &getVertexList() const { return vertexList; }

private:
    QVector<Vec3d> vertexList;
};