#include <QtGui/qvector2d.h>
#include "renderer.h"

Renderer::Renderer(quint32 width, quint32 height)
        : width(width), height(height), image(NULL)
{

}

QPixmap Renderer::render() {
    if (!image) {
        image = new QImage(width, height, QImage::Format_RGB888);
        image->fill(qRgb(0, 0, 0));
    }

    return QPixmap::fromImage(*image);
}

void Renderer::renderLine(Vec2i p0, Vec2i p1, uint color) {
    int x0 = p0[0];
    int y0 = p0[1];
    int x1 = p1[0];
    int y1 = p1[1];

    bool swapped = false;
    if (qAbs(x0 - x1) < qAbs(y0 - y1)) {
        qSwap(x0, y0);
        qSwap(x1, y1);
        swapped = true;
    }

    if (x0 > x1) {
        qSwap(x0, x1);
        qSwap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;

    int de = qAbs(dy) * 2;
    int e = 0;

    int incY = (y1 > y0 ? 1 : -1);

    for (int x = x0, y = y0; x <= x1; x++) {
        if (swapped) {
            image->setPixel(y, x, color);
        } else {
            image->setPixel(x, y, color);
        }
        e += de;

        if (e > dx) {
            y += incY;
            e -= 2 * dx;
        }
    }
}
