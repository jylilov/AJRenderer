#pragma once

#include <QPixmap>
#include "vector.h"

class Renderer
{
private:
    uint width;
    uint height;

    QImage *image;

public:
    Renderer(uint width, uint height);

    QPixmap render();

private:
    void renderLine(Vec2i p0, Vec2i p1, uint color);


};