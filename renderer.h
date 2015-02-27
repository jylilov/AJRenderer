#pragma once

#include <QPixmap>
#include "vector.h"
#include "objectmodel.h"

class Renderer
{
private:
    uint width;
    uint height;

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
    void renderLine(int x0, int y0, int x1, int y1, uint color);
    void renderLine(Vec2i p0, Vec2i p1, uint color);

    void renderLine(Vec4d p0, Vec4d p1, uint color);
    void renderTriangle(Triangle &t);

    void renderObject(ObjectModel *object);

    void putPixel(int x, int y, uint color);

    bool isOnScreen(int x, int y);

    Vec4d transform(Vec4d p0);
};