#pragma once

#include <QPixmap>
#include "vector.h"
#include "warefrontobject.h"

class Renderer
{
private:
    uint width;
    uint height;

    QImage *image;

    QList<WarefrontObject *> objects;

public:
    Renderer(uint width, uint height);

    QPixmap render();
    void addObject(WarefrontObject *object) { objects.append(object); }
    void removeObject(WarefrontObject *object) { objects.removeOne(object); }

private:
    void renderLine(int x0, int y0, int x1, int y1, uint color);
    void renderLine(Vec2i p0, Vec2i p1, uint color);
    void renderLine(Vec4d p0, Vec4d p1, uint color);
    void renderTriangle(Vec4d &v1, Vec4d &v2, Vec4d &v3);
    void renderObject(WarefrontObject *object);
};