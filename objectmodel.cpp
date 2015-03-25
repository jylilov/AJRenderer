#include "objectmodel.h"

void ObjectModel::addTriangles(ObjectModel *object, QList< Vector<3, Vec3i> > &triangles) {
    Vector<3, Vec3i> triangle;
            foreach (triangle, triangles) {
            Vector<3, Vertex> newTriangle;
            for (uint i = 0; i < 3; ++i) {
                Vertex vertex;
                vertex.setCoordinates(object->vertexList[triangle[i][0]]);
                vertex.setTextureCoordinates(object->textureVertexList[triangle[i][1]]);
                vertex.setNormalVector(object->normalVectors[triangle[i][2]]);
                newTriangle[i] = vertex;
            }
            object->triangles.push_back(newTriangle);
        }
}

Vector<3, Vec3i> ObjectModel::parseTriangle(QList<QString>::const_iterator &iterator) {
    Vector<3, Vec3i> triangle;
    for (uint i = 0; i < 3; ++i) {
        ++iterator;
        Vec3i vertex;
        QStringList array = iterator->split('/');

        QStringList::const_iterator j = array.constBegin();

        for (uint k = 0; k < 3; ++k) {
            vertex[k] = (j++)->toInt() - 1;
        }

        triangle[i] = vertex;

    }
    return triangle;
}

Vec2d ObjectModel::parseTextureVector(QList<QString>::const_iterator &iterator) {
    Vec2d vector;
    for (int i = 0; i < 2; ++i) {
        ++iterator;
        vector[i] = iterator->toDouble();
    }
    vector[1] = 1 - vector[1];
    return vector;
}

Vec3d ObjectModel::parseVector(QList<QString>::const_iterator &iterator) {
    Vec3d vector;
    for (int i = 0; i < 3; ++i) {
        ++iterator;
        vector[i] = iterator->toDouble();
    }
    vector[1] *= -1;
    return vector;
}

ObjectModel *ObjectModel::fromWareFrontObjectFile(QString file_path) {
    ObjectModel *object = new ObjectModel();
    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly))
        return 0;

    QList< Vector<3, Vec3i> > triangles;

    while (!file.atEnd()) {
        QString line(file.readLine());

        line.remove('\n');
        QStringList words = line.split(QRegExp("[ ]+"));

        QStringList::const_iterator iterator = words.constBegin();
        QString mode = *iterator;

        if (mode == "v") {
            Vec3d vector = parseVector(iterator);
            object->vertexList.push_back(vector);
        } else if (mode == "vt") {
            Vec2d vector = parseTextureVector(iterator);
            object->textureVertexList.push_back(vector);
        } else if (mode == "vn") {
            Vec3d vector = parseVector(iterator);
            object->normalVectors.push_back(vector);
        } else if (mode == "f") {
            Vector<3, Vec3i> triangle = parseTriangle(iterator);
            triangles.push_back(triangle);
        }
    }

    file.close();
    addTriangles(object, triangles);

    return object;
}

void ObjectModel::updateMatrix() {
    Mat4d turnX(
            1.0, 0.0, 0.0, 0.0,
            0.0, qCos(direction[0]), -qSin(direction[0]), 0.0,
            0.0, qSin(direction[0]), qCos(direction[0]), 0.0,
            0.0, 0.0, 0.0, 1.0
    );

    Mat4d turnY(
            qCos(direction[1]), 0.0, qSin(direction[1]), 0.0,
            0.0, 1.0, 0.0, 0.0,
            -qSin(direction[1]), 0.0, qCos(direction[1]), 0.0,
            0.0, 0.0, 0.0, 1.0
    );

    Mat4d turnZ(
            qCos(direction[2]), -qSin(direction[2]), 0.0, 0.0,
            qSin(direction[2]), qCos(direction[2]), 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
    );

    modelMatrix = Mat4d::getScaleMatrix(size) * turnX * turnY * turnZ;
}
