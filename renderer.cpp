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

bool Renderer::isOnScreen(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Renderer::renderObject(ObjectModel *object) {
    QVector<Triangle> triangles = object->getTrianglesList();

    for (QVector<Triangle>::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        renderTriangle(*i, object->getTexture());
    }
}

void Renderer::renderTriangle(Triangle &t, Texture *texture) {

    Vec4d v1 = t.getVertex1();
    Vec4d v2 = t.getVertex2();
    Vec4d v3 = t.getVertex3();

    double z = (v1[2] + v2[2] + v3[2]) / 3;

    double perspectiveArray[] = {
            width / 2 * 3 / 4, 0, 0, width / 8 + width / 2 * 3 / 4,
            0, height / 2 * 3 / 4, 0, height / 8 + height / 2 * 3 / 4,
            0, 0, 1, 0,
            0, 0, 0, 1,
    };

    double projArray[] = {
            qCos(-0.5), 0, -qSin(-0.5), 0,
            0, 1, 0, 0,
            qSin(-0.5), 0, qCos(-0.5), 0,
            0, 0, -0.33, 1
    };

    Mat4d perspectiveMatrix(perspectiveArray);
    Mat4d projMatrix(projArray);

    v1 = perspectiveMatrix * (projMatrix * v1);
    v2 = perspectiveMatrix * (projMatrix * v2);
    v3 = perspectiveMatrix * (projMatrix * v3);

    v1 = v1 * (1 / v1[3]);
    v2 = v2 * (1 / v2[3]);
    v3 = v3 * (1 / v3[3]);

    int minX = qRound(qMin(v1[0], qMin(v2[0], v3[0])));
    int maxX = qRound(qMax(v1[0], qMax(v2[0], v3[0])));
    int minY = qRound(qMin(v1[1], qMin(v2[1], v3[1])));
    int maxY = qRound(qMax(v1[1], qMax(v2[1], v3[1])));

    double matrixArray[] = {
            qRound(v1[0]), qRound(v2[0]), qRound(v3[0]),
            qRound(v1[1]), qRound(v2[1]), qRound(v3[1]),
            1, 1, 1
    };

    Vec3d vt1 = t.getTextureVertex1();
    Vec3d vt2 = t.getTextureVertex2();
    Vec3d vt3 = t.getTextureVertex3();

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

    //curVertex[2] = 1;
    uint color = qRgb((z + 1) * 128, (z + 1) * 128, (z + 1) * 128);

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j < maxY; ++j) {
            curVertex[0] = i;
            curVertex[1] = j;
            curVertex[2] = 1;
            barycentric = matrix * curVertex;
            if (barycentric[0] > -1e-10 && barycentric[1] > -1e-10 & barycentric[2] > -1e-10) {
                if (z > zBuffer[i * width + j]) {
                    if (texture) {
                        Vec3d textureCoordinates = textureMatrix * barycentric;
                        color = texture->getColor(textureCoordinates[0], textureCoordinates[1]);
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