#pragma once

#include <QPixmap>
#include "vector.h"
#include "objectmodel.h"
#include "shader.h"

class Renderer
{
public:
    static Mat4d getViewMatrix(Vec3d eye, Vec3d center, Vec3d up);
    static Mat4d getProjectionMatrix(double coefficient);
    static Mat4d getViewportMatrix(uint width, uint height);
    static Vec3d getBarycentricCoordinate(Vec3d coordinates[3], Vec3i point);
    static void drawTriangle(Vec4d t[3], IShader *shader, QImage *frame, Buffer *zBuffer);
    static void drawPixel(int const &x, int const &y, VecColor const &color, QImage *frame);
protected:
    uint width;
    uint height;

    Vec3d lightVector;
    Vec3d eye;
    Vec3d center;
    Vec3d up;

    Mat4d viewport;
    Mat4d projection;
    Mat4d view;
    Mat4d shadowMatrix;

    QImage *image;
    Buffer *zBuffer;
    Buffer *shadowBuffer;

    QList<ObjectModel *> objects;
public:
    Renderer(uint width, uint height);
    ~Renderer() {}
    QPixmap render();
    void addObject(ObjectModel *object) { objects.append(object); }
    void removeObject(ObjectModel *object) { objects.removeOne(object); }
private:
    void drawObject(ObjectModel *object);
    void calcObjectShadow(ObjectModel *object);
};