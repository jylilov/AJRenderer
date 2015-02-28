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

    return QPixmap::fromImage(image);
}

bool Renderer::isOnScreen(int x, int y) {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Renderer::renderObject(ObjectModel *object) {
    QVector<Vector<3, Vertex> > triangles = object->getTriangles();

    Vec3d lightVector( 1.0, 0.0, 0.0 );
    lightVector.normalize();

    objectMatrix = object->getModelMatrix();
    texture = object->getTexture();

    for (QVector<Vector<3, Vertex> >::iterator i = triangles.begin(); i != triangles.end(); ++i) {
        Vec3i v[3];

        for (size_t j = 0; j < 3; ++j) {
            Vertex vertex = (*i)[j];
            textureCoordinates[0][j] = vertex.getTextureCoordinates()[0];
            textureCoordinates[1][j] = vertex.getTextureCoordinates()[1];
            intensity[j] = vertex.getNormalVector() * lightVector;
            v[j] = vertexShader(vertex);
        }

        renderTriangle(v);
    }

}

void Renderer::renderTriangle(Vec3i t[3], Texture *texture) {
    int minX = std::numeric_limits<int>::max();
    int maxX = -std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxY = -std::numeric_limits<int>::max();

    for (int i = 0; i < 3; ++i) {
        minX = qMin(minX, t[i][0]);
        maxX = qMax(maxX, t[i][0]);
        minY = qMin(minY, t[i][1]);
        maxY = qMax(maxY, t[i][1]);
    }

    minX = qMax(minX, 0);
    maxX = qMin(maxX, int(width));
    minY = qMax(minY, 0);
    maxY = qMin(maxY, int(height));

    Vec3d barycentric;
    Vec3i curVertex;

    double z = (t[1][2] + t[2][2] + t[3][2]) / 3; //TODO right zBuffer calculation

    for (int i = minX; i <= maxX; ++i) {
        for (int j = minY; j < maxY; ++j) {
            curVertex[0] = i;
            curVertex[1] = j;
            curVertex[2] = 1;

            barycentric = getBarycentricCoordinate(t, curVertex);

            if (barycentric[0] > -1e-10 && barycentric[1] > -1e-10 & barycentric[2] > -1e-10) {
                if (z > zBuffer[i * width + j]) {
                    uint color;
                    if (!fragmentShader(barycentric, color)) {
                        zBuffer[i * width + j] = z;
                        putPixel(i, j, color);
                    }
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


    Mat4d perspective(
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, -0.001, 1.0
    );

    sceneMatrix = sceneMatrix * perspective;
}

Vec3i Renderer::vertexShader(Vertex vertex) {
    Vec4d coordinates = Vec4d::cast(vertex.getCoordinates());
    coordinates[3] = 1;

    coordinates = sceneMatrix * objectMatrix * coordinates;
    coordinates = coordinates / coordinates[3];
    Vec3i answer = Vec3i::cast(Vec3d::cast(coordinates));

    return answer;
}

Vec3d Renderer::getBarycentricCoordinate(Vec3i coordinates[3], Vec3i point) {
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

bool Renderer::fragmentShader(Vec3d v, uint &color) {
    double intensity = (v * this->intensity + 1) / 2;
    double x = textureCoordinates[0] * v;
    double y = textureCoordinates[1] * v;
    color = texture->getColor(x, y);
    int r = qRed(color) * intensity;
    int g = qGreen(color) * intensity;
    int b = qBlue(color) * intensity;
    color = qRgb(r, g, b);
    return false;
}
