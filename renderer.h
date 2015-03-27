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
    static Vec3d getBarycentricCoordinate(Vec3d const coordinates[3], Vec3i const &point);
    static void drawTriangle(Vec4d triangle[3], AbstractShader *shader, QImage *frame, Buffer *zBuffer);
    static void drawPixel(int const &x, int const &y, VecColor const &color, QImage *frame);
protected:
    uint width;
    uint height;

    Vec3d lightVector;
    Vec3d eyeVector;
    Vec3d centerVector;
    Vec3d upVector;

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
    void removeObject(int index) { objects.removeOne(objects[index]); }
    ObjectModel *getObject(int index) { return objects[index]; }

    Vec3d getLightVector() const { return lightVector; }
    void setLightVector(Vec3d const &lightVector) { this->lightVector = lightVector.getNormalVector(); }
    Vec3d getEyeVector() const { return eyeVector; }
    void setEyeVector(Vec3d const &eyeVector) { this->eyeVector = eyeVector; }
    Vec3d getCenterVector() const { return centerVector; }
    void setCenterVector(Vec3d const &centerVector) { Renderer::centerVector = centerVector; }
    Vec3d getUpVector() const { return upVector; }
    void setUpVector(Vec3d const &upVector) { Renderer::upVector = upVector.getNormalVector();  }

private:
    void drawObject(ObjectModel *object);
    void calcObjectShadow(ObjectModel *object);
};