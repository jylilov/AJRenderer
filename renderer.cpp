#include "renderer.h"

#include "matrix.h"

Renderer::Renderer(quint32 width, quint32 height)
        : width(width), height(height), image(width, height, QImage::Format_RGB888)
{
    zBuffer = new double[width * height];
}

QPixmap Renderer::render() {
    image.fill(qRgb(0, 0, 0));
    for (int i = 0; i < width * height; ++i)
        zBuffer[i] = -std::numeric_limits<double>::max();

    for (QList<ObjectModel *>::iterator i = objects.begin(); i != objects.end(); ++i) {
        renderObject(*i);
    }

    return QPixmap::fromImage(image);
}

//TODO remove renderLine methods
void Renderer::renderLine(Vec2i p0, Vec2i p1, uint color) {
    renderLine(p0[0], p0[1], p1[0],p1[1], color);
}

void Renderer::renderLine(Vec4d p0, Vec4d p1, uint color) {
    /*Vec2i pi0 = transorm(p0);
    Vec2i pi1 = transorm(p1);

    if (isOnScreen(pi0) || isOnScreen(pi1)) {
        renderLine(pi0, pi1, color);
    }*/
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
            putPixel(y, x, color);
        } else {
            putPixel(x, y, color);
        }
        e += de;

        if (e > dx) {
            y += incY;
            e -= 2 * dx;
        }
    }
}

Vec4d Renderer::transform(Vec4d v) {
    Vec4d newV = v;
    newV[0] = qRound((v[0] + 1) * width / 2);
    newV[1] = qRound((v[1] + 1) * height / 2);
    return newV;
}


//TODO remove if not needed
bool Renderer::isOnScreen(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Renderer::renderObject(ObjectModel *object) {
    QVector<Triangle> triangles = object->getTrianglesList();

    for (QVector<Triangle>::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        Triangle t = *i;

        for (size_t j = 0; j < 3; ++j) {
            t.setVertex(j, transform(t.getVertex(j)));
        }

        renderTriangle(t, object->getTexture());
    }
}

void Renderer::renderTriangle(Triangle &t, Texture *texture) {
    Vec4d v1 = t.getVertex1();
    Vec4d v2 = t.getVertex2();
    Vec4d v3 = t.getVertex3();

    Vec3d vt1 = t.getTextureVertex1();
    Vec3d vt2 = t.getTextureVertex2();
    Vec3d vt3 = t.getTextureVertex3();

    double z = (v1[2] + v2[2] + v3[2]) / 3;

    int minX = qRound(qMin(v1[0], qMin(v2[0], v3[0])));
    int maxX = qRound(qMax(v1[0], qMax(v2[0], v3[0])));
    int minY = qRound(qMin(v1[1], qMin(v2[1], v3[1])));
    int maxY = qRound(qMax(v1[1], qMax(v2[1], v3[1])));

    double matrixArray[] = {
            v1[0], v2[0], v3[0],
            v1[1], v2[1], v3[1],
            1, 1, 1
    };

    double textureMatrixArray[] = {
            vt1[0], vt2[0], vt3[0],
            vt1[1], vt2[1], vt3[1],
            1, 1, 1
    };

    Mat3d matrix(matrixArray);
    matrix = matrix.getInverseMatrix();

    Mat3d textureMatrix(textureMatrixArray);

    Vec3d barycentric;
    Vec3d curVertex;

    curVertex[2] = 1;
    uint color = qRgb((z + 1) * 128, (z + 1) * 128, (z + 1) * 128);

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j < maxY; ++j) {
            curVertex[0] = i;
            curVertex[1] = j;
            barycentric = matrix * curVertex;
            if (barycentric[0] < -1e-10 || barycentric[1] < -1e-10 || barycentric[2] < -1e-10)
                continue;
            else {
                if (z > zBuffer[i * width + j]) {
                    if (texture) {
                        Vec3d textureCoordinats = textureMatrix * barycentric;
                        color = texture->getColor(textureCoordinats[0], textureCoordinats[1]);
                    }
                    putPixel(i, j, color);

                    zBuffer[i * width + j] = z;
                }
            }
        }
    }
}

void Renderer::putPixel(int x, int y, uint color) {
    if (isOnScreen(x, y))
        image.setPixel(x, y, color);
}