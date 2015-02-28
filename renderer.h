#pragma once

#include <QPixmap>
#include "vector.h"
#include "objectmodel.h"

class Renderer
{
private:
    uint width;
    uint height;

    Vec3d intensity;
    Vec3d textureCoordinates[2];
    Texture *texture;

    Mat4d objectMatrix;
    Mat4d sceneMatrix;

    QImage image;
    double *zBuffer;

    QList<ObjectModel *> objects;
public:
    Renderer(uint width, uint height);
    ~Renderer() { delete [] zBuffer; }

    QPixmap render();
    void addObject(ObjectModel *object) { objects.append(object); }
    void removeObject(ObjectModel *object) { objects.removeOne(object); }
private:
    void renderTriangle(Vec3i t[3], Texture *text = 0);
    void renderObject(ObjectModel *object);
    void putPixel(int x, int y, uint color);
    bool isOnScreen(int x, int y);
    Vec3i vertexShader(Vertex vertex);
    bool fragmentShader(Vec3d v, uint &color);

    Vec3d getBarycentricCoordinate(Vec3i coordinates[3], Vec3i point);

    void updateMatrix();
};