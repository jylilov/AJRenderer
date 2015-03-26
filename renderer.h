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
    void removeObject(int index) { objects.removeOne(objects[index]); }
    ObjectModel *getObject(int index) { return objects[index]; }


    Vec3d getLightVector() const {
        return lightVector;
    }

    void setLightVector(Vec3d const &lightVector) {
        Renderer::lightVector = lightVector.getNormalVector();
    }

    Vec3d getEye() const {
        return eye;
    }

    void setEye(Vec3d const &eye) {
        Renderer::eye = eye;
    }

    Vec3d getCenter() const {
        return center;
    }

    void setCenter(Vec3d const &center) {
        Renderer::center = center;
    }

    Vec3d getUp() const {
        return up;
    }

    void setUp(Vec3d const &up) {
        Renderer::up = up.getNormalVector();
    }

private:
    void drawObject(ObjectModel *object);
    void calcObjectShadow(ObjectModel *object);
};