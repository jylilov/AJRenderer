#include <iostream>
#include "warefrontobject.h"
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

    for (QList<WarefrontObject *>::iterator i = objects.begin(); i != objects.end(); ++i) {
        renderObject(*i);
    }

    return QPixmap::fromImage(*image);
}

void Renderer::renderLine(Vec2i p0, Vec2i p1, uint color) {
    renderLine(p0[0], p0[1], p1[0],p1[1], color);
}

void Renderer::renderLine(Vec4d p0, Vec4d p1, uint color) {
    Vec2i pi0;
    pi0[0] = qRound((p0[0] + 1) * 299.5);
    pi0[1] = qRound((p0[1] + 1) * 299.5);
    Vec2i pi1;
    pi1[0] = qRound((p1[0] + 1) * 299.5);
    pi1[1] = qRound((p1[1] + 1) * 299.5);
    renderLine(pi0, pi1, color);
}

void Renderer::renderLine(int x0, int y0, int x1, int y1, uint color) {
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

void Renderer::renderObject(WarefrontObject *object) {
    QVector<Vec3i> triangles = object->getTrianglesList();
    QVector<Vec4d> vertices = object->getVertexList();

    for (QVector<Vec3i>::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        Vec4d v1, v2, v3;

        v1 = vertices.at((*i)[0]);
        v2 = vertices.at((*i)[1]);
        v3 = vertices.at((*i)[2]);

        renderTriangle(v1, v2, v3);
    }
}

void Renderer::renderTriangle(Vec4d &v1, Vec4d &v2, Vec4d &v3) {
    renderLine(v1, v2, qRgb(255, 255, 255));
    renderLine(v1, v3, qRgb(255, 255, 255));
    renderLine(v3, v2, qRgb(255, 255, 255));
}
