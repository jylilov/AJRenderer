#include "renderer.h"

Mat4d Renderer::getViewMatrix(Vec3d eye, Vec3d center, Vec3d up) {
    Vec3d z = (eye - center).getNormalVector();
    Vec3d x = cross(up, z).getNormalVector();
    Vec3d y = cross(z, x).getNormalVector();

    Mat4d turn = Mat4d::getIdentityMatrix();
    turn[0] = Vec4d::cast(x);
    turn[1] = Vec4d::cast(y);
    turn[2] = Vec4d::cast(z);

    Mat4d translate = Mat4d::getIdentityMatrix();
    for (uint i = 0; i < 3; ++i) {
        translate[i][3] = - center[i];
    }

    return turn * translate;
}

Mat4d Renderer::getProjectionMatrix(double coefficient) {
    Mat4d projection = Mat4d::getIdentityMatrix();
    projection[3][2] = coefficient;
    return projection;
}

Mat4d Renderer::getViewportMatrix(uint width, uint height) {
    Mat4d viewport = Mat4d::getIdentityMatrix();
    viewport[0][3] = width / 2;
    viewport[1][3] = height / 2;
    viewport[0][0] = width / 2;
    viewport[1][1] = height / 2;
    viewport[2][3] = 128;
    viewport[2][2] = 128;
    return viewport;
}

Renderer::Renderer(uint width, uint height)
        : width(width), height(height)
{
    // TODO move to constants
    lightVector = Vec3d(1.0, -1.0, 1.0).getNormalVector();
    up = Vec3d(0.0, 1.0, 0.0).getNormalVector();
    center = Vec3d(0.0, 0.0, 0.0);
    eye = Vec3d(0.0, 0.0, 3.0);
}

QPixmap Renderer::render() {
    uint width = this->width;
    uint height = this->height;

    zBuffer = Buffer::createBuffer(width, height);
    shadowBuffer = Buffer::createBuffer(width, height);

    image = new QImage(width, height, QImage::Format_RGB888);
    image->fill(qRgb(0, 0, 0));

    viewport = getViewportMatrix(width, height);
    projection = getProjectionMatrix(0);
    view = getViewMatrix(lightVector, center, up);

    shadowMatrix = viewport * projection * view;

    for (QList<ObjectModel *>::const_iterator i = objects.constBegin(); i != objects.constEnd(); ++i)
        calcObjectShadow(*i);

    projection = getProjectionMatrix(-1 / (eye - center).getLength());
    view = getViewMatrix(eye, center, up);

    for (QList<ObjectModel *>::const_iterator i = objects.constBegin(); i != objects.constEnd(); ++i)
        drawObject(*i);

    QPixmap ans = QPixmap::fromImage(*image);

    delete zBuffer;
    delete shadowBuffer;
    delete image;

    return ans;
}

void Renderer::calcObjectShadow(ObjectModel *object) {
    ShadowShader *shadowShader = new ShadowShader();
    shadowShader->setUniformShadowMatrix(shadowMatrix * object->getModelMatrix());

    QVector< Vector<3, Vertex> > triangles = object->getTriangles();

    Vector<3, Vertex> triangle;
    foreach(triangle, triangles) {
        Vec4d vertexCoordinates[3];
        for (uint i = 0; i < 3; ++i) {
            Vertex vertex = triangle[i];
            vertexCoordinates[i] = shadowShader->vertexShader(vertex, i);
        }
        drawTriangle(vertexCoordinates, shadowShader, image, shadowBuffer);
    }

    delete shadowShader;
}

void Renderer::drawObject(ObjectModel *object) {
    Shader *shader = new Shader();

    shader->setUniformViewport(viewport);
    shader->setUniformProjection(projection);
    shader->setUniformView(view);
    shader->setUniformModel(object->getModelMatrix());
    shader->setUniformShadow(shadowMatrix);
    shader->setUniformShadowBuffer(shadowBuffer);
    shader->setUniformLightVector(lightVector);
    shader->setUniformDiffTexture(object->getDiffuseTexture());
    shader->setUniformNmTexture(object->getNmTexture());
    shader->setUniformSpecTexture(object->getSpecTexture());
    shader->update();


    QVector<Vector<3, Vertex> > triangles = object->getTriangles();
    Vector<3, Vertex> triangle;
    foreach(triangle, triangles) {
        Vec4d vertexCoordinates[3];

        for (uint i = 0; i < 3; ++i) {
            Vertex vertex = triangle[i];
            vertexCoordinates[i] = shader->vertexShader(vertex, i);
        }

        drawTriangle(vertexCoordinates, shader, image, zBuffer);
    }

    delete shader;
}

void Renderer::drawTriangle(Vec4d triangle[3], AbstractShader *shader, QImage *frame, Buffer *zBuffer) {
    Vec3d vertexCoordinates[3];
    for (uint i = 0; i < 3; ++i) {
        vertexCoordinates[i] = triangle[i].getProjection();
    }

    int minX = std::numeric_limits<int>::max();
    int maxX = -std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxY = -std::numeric_limits<int>::max();

    for (int i = 0; i < 3; ++i) {
        minX = qMin(minX, qFloor(vertexCoordinates[i][0]));
        maxX = qMax(maxX, qCeil(vertexCoordinates[i][0]));
        minY = qMin(minY, qFloor(vertexCoordinates[i][1]));
        maxY = qMax(maxY, qCeil(vertexCoordinates[i][1]));
    }

    minX = qMax(minX, 0);
    maxX = qMin(maxX, frame->width());
    minY = qMax(minY, 0);
    maxY = qMin(maxY, frame->height());

    Vec3d bcScene;
    Vec3d bcModel;

    double z;

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j < maxY; ++j) {
            bcScene = getBarycentricCoordinate(vertexCoordinates, Vec3i(i, j, 1));
            bcModel = Vec3d(bcScene[0]/ triangle[0][3], bcScene[1]/ triangle[1][3], bcScene[2]/ triangle[2][3]);

            bcModel = bcModel / (bcModel[0] + bcModel[1] + bcModel[2]);

            if (bcScene[0] > 0 && bcScene[1] > 0 & bcScene[2] > 0) {
                z = Vec3d(vertexCoordinates[0][2], vertexCoordinates[1][2], vertexCoordinates[2][2]) * bcScene;
                if (z > zBuffer->get(i, j)[0]) {
                    VecColor color;
                    if (!shader->fragmentShader(bcModel, color)) {
                        zBuffer->set(i, j, static_cast<uchar>(z));
                        drawPixel(i, j, color, frame);
                    }
                }
            }
        }
    }
}

void Renderer::drawPixel(int const &x, int const &y, const VecColor &color, QImage *frame) {
    frame->setPixel(x, y, qRgb(color[0], color[1], color[2]));
}

Vec3d Renderer::getBarycentricCoordinate(Vec3d const coordinates[3], Vec3i const &point) {
    Mat3d matrix;
    for (int i = 0; i < 3; ++i) {
        matrix[0][i] = coordinates[i][0];
        matrix[1][i] = coordinates[i][1];
        matrix[2][i] = 1;
    }
    matrix = matrix.getInverseMatrix();
    Vec3d answer = matrix * Vec3d::cast(point);
    return answer;
}
