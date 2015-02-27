#include "renderer.h"

Renderer::Renderer(quint32 width, quint32 height)
        : width(width), height(height), image(width, height, QImage::Format_RGB888)
{
    zBuffer = new double[width * height];
    updateMatrix();
}

QPixmap Renderer::render() {
    image.fill(qRgb(0, 0, 0));
    for (int i = 0; i < width * height; ++i)
        zBuffer[i] = -std::numeric_limits<double>::max();

    for (QList<ObjectModel *>::iterator i = objects.begin(); i != objects.end(); ++i) {
        renderObject(*i);
    }

    time += 0.1;

    return QPixmap::fromImage(image);
}

bool Renderer::isOnScreen(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Renderer::renderObject(ObjectModel *object) {
    QVector<Triangle> triangles = object->getTrianglesList();

    objectMatrix = object->getModelMatrix();

    for (QVector<Triangle>::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        renderTriangle(*i, object->getTexture());
    }
}

void Renderer::renderTriangle(Triangle &t, Texture *texture) {

    Vec4d v1 = sceneMatrix * objectMatrix * t.getVertex1();
    Vec4d v2 = sceneMatrix * objectMatrix * t.getVertex2();
    Vec4d v3 = sceneMatrix * objectMatrix * t.getVertex3();

    v1 = v1 / v1[3];
    v2 = v2 / v2[3];
    v3 = v3 / v3[3];

    int minX = qMax(qRound(qMin(v1[0], qMin(v2[0], v3[0]))), 0);
    int maxX = qMin(qRound(qMax(v1[0], qMax(v2[0], v3[0]))), (int)width);
    int minY = qMax(qRound(qMin(v1[1], qMin(v2[1], v3[1]))), 0);
    int maxY = qMin(qRound(qMax(v1[1], qMax(v2[1], v3[1]))), (int)height);

    double matrixArray[] = {
            qRound(v1[0]), qRound(v2[0]), qRound(v3[0]),
            qRound(v1[1]), qRound(v2[1]), qRound(v3[1]),
            1, 1, 1
    };

    Mat3d matrix(matrixArray);
    matrix = matrix.getInverseMatrix();

    Vec3d vt1 = t.getTextureVertex1();
    Vec3d vt2 = t.getTextureVertex2();
    Vec3d vt3 = t.getTextureVertex3();

    double textureMatrixArray[] = {
            vt1[0], vt2[0], vt3[0],
            vt1[1], vt2[1], vt3[1],
            1, 1, 1
    };

    Mat3d textureMatrix(textureMatrixArray);

    Vec3d barycentric;
    Vec3d curVertex;

    double z = (v1[2] + v2[2] + v3[2]) / 3; //TODO right zBuffer calculation

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j < maxY; ++j) {
            curVertex[0] = i;
            curVertex[1] = j;
            curVertex[2] = 1;

            barycentric = matrix * curVertex;
            if (barycentric[0] > -1e-10 && barycentric[1] > -1e-10 & barycentric[2] > -1e-10) {
                if (z > zBuffer[i * width + j]) {
                    Vec3d textureCoordinates = textureMatrix * barycentric;
                    putPixel(i, j, texture->getColor(textureCoordinates[0], textureCoordinates[1]));

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

void Renderer::updateMatrix() {
    sceneMatrix = Mat4d::getIdentityMatrix();
    sceneMatrix[0][3] = width / 2;
    sceneMatrix[1][3] = height / 2;


    double array[] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0,  0, -0.001, 1
    };

    sceneMatrix = sceneMatrix * Mat4d(array) ;
}
