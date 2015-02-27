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
    void renderTriangle(Triangle &t, Texture *text);

    void renderObject(ObjectModel *object);

    void putPixel(int x, int y, uint color);

    bool isOnScreen(int x, int y);

    Vec4d transform(Vec4d p0);
};