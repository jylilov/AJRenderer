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
        : width(width), height(height), anti_aliasing(2)
{
    // TODO move to constants
    lightVector = Vec3d(1.0, -1.0, 1.0).getNormalVector();
    up = Vec3d(0.0, 1.0, 0.0).getNormalVector();
    center = Vec3d(0.0, 0.0, 0.0);
    eye = Vec3d(0.0, 0.0, 3.0);
}

QPixmap Renderer::render() {
    uint width = this->width * anti_aliasing;
    uint height = this->height * anti_aliasing;

    zBuffer = Buffer::createBuffer(width, height);
    shadowBuffer = Buffer::createBuffer(width, height);
    image = new QImage(width, height, QImage::Format_RGB888);

    image->fill(qRgb(0, 0, 0));
    shadowBuffer->fill(0);
    zBuffer->fill(0);

    viewport = getViewportMatrix(width, height);
    projection = getProjectionMatrix(0);
    view = getViewMatrix(lightVector, center, up);

    shadowMatrix = viewport * projection * view;

    for (QList<ObjectModel *>::const_iterator i = objects.constBegin(); i != objects.constEnd(); ++i)
        calcObjectShadow(*i);

    viewport = getViewportMatrix(width, height);
    projection = getProjectionMatrix(-1 / (eye - center).getLength());
    view = getViewMatrix(eye, center, up);

    for (QList<ObjectModel *>::const_iterator i = objects.constBegin(); i != objects.constEnd(); ++i)
        drawObject(*i);

    QPixmap ans = QPixmap::fromImage(resize(image));

    delete zBuffer;
    delete shadowBuffer;
    delete image;

    return ans;
}

QImage Renderer::resize(QImage *image) {
    QImage result(this->width, this->height, QImage::Format_RGB888);
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            int r = 0;
            int g = 0;
            int b = 0;
            for (int ii = 0; ii < anti_aliasing; ++ii) {
                for (int jj = 0; jj < anti_aliasing; ++jj) {
                    r += qRed(image->pixel(i * anti_aliasing + ii, j * anti_aliasing + jj));
                    g += qGreen(image->pixel(i * anti_aliasing + ii, j * anti_aliasing + jj));
                    b += qBlue(image->pixel(i * anti_aliasing + ii, j * anti_aliasing + jj));
                }
            }
            r /= anti_aliasing * anti_aliasing;
            g /= anti_aliasing * anti_aliasing;
            b /= anti_aliasing * anti_aliasing;

            result.setPixel(i, j, qRgb(r, g, b));
        }
    }
    return result;
}

void Renderer::calcObjectShadow(ObjectModel *object) {
    ShadowShader *shadowShader = new ShadowShader(
            shadowMatrix * object->getModelMatrix()
    );

    QVector<Vector<3, Vertex> > triangles = object->getTriangles();
    for (QVector<Vector<3, Vertex> >::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        Vec4d v[3];

        for (size_t j = 0; j < 3; ++j) {
            Vertex vertex = (*i)[j];
            v[j] = shadowShader->vertexShader(vertex);
        }

        drawTriangle(v, shadowShader, image, shadowBuffer);
        shadowShader->nextTriangle();
    }

    delete shadowShader;
}

void Renderer::drawObject(ObjectModel *object) {
    Shader *shader = new Shader(
            viewport,
            projection,
            view,
            object->getModelMatrix(),
            shadowMatrix,
            shadowBuffer,
            lightVector,
            object->getDiffuseTexture(),
            object->getNmTexture(),
            object->getSpecTexture()
    );

    QVector<Vector<3, Vertex> > triangles = object->getTriangles();
    for (QVector<Vector<3, Vertex> >::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        Vec4d v[3];

        for (size_t j = 0; j < 3; ++j) {
            Vertex vertex = (*i)[j];
            v[j] = shader->vertexShader(vertex);
        }

        drawTriangle(v, shader, image, zBuffer);
        shader->nextTriangle();
    }

    delete shader;
}

void Renderer::drawTriangle(Vec4d t[3], IShader *shader, QImage *frame, Buffer *zBuffer) {
    Vec3d tt[3];
    for (uint i = 0; i < 3; ++i) {
        tt[i] = t[i].getProjection();
    }

    int minX = std::numeric_limits<int>::max();
    int maxX = -std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxY = -std::numeric_limits<int>::max();

    for (int i = 0; i < 3; ++i) {
        minX = qMin(minX, qFloor(tt[i][0]));
        maxX = qMax(maxX, qCeil(tt[i][0]));
        minY = qMin(minY, qFloor(tt[i][1]));
        maxY = qMax(maxY, qCeil(tt[i][1]));
    }

    minX = qMax(minX, 0);
    maxX = qMin(maxX, frame->width());
    minY = qMax(minY, 0);
    maxY = qMin(maxY, frame->height());

    Vec3d barycentric;
    Vec3d bc_clip;

    double z;

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j < maxY; ++j) {
            barycentric = getBarycentricCoordinate(tt, Vec3i(i, j, 1));
            bc_clip = Vec3d(barycentric[0]/t[0][3], barycentric[1]/t[1][3], barycentric[2]/t[2][3]);

            bc_clip = bc_clip / (bc_clip[0] + bc_clip[1] + bc_clip[2]);

            if (barycentric[0] > 0 && barycentric[1] > 0 & barycentric[2] > 0) {
                z = Vec3d(tt[0][2], tt[1][2], tt[2][2]) * barycentric;
                if (z > zBuffer->get(i, j)[0]) {
                    VecColor color;
                    if (!shader->fragmentShader(bc_clip, color)) {
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

Vec3d Renderer::getBarycentricCoordinate(Vec3d coordinates[3], Vec3i point) {
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
