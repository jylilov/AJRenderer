#include "objectmodel.h"

ObjectModel *ObjectModel::fromWareFrontObjectFile(QString file_path) {
    ObjectModel *object = new ObjectModel();
    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly))
        return 0;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();

        line.remove(line.size() - 1, 1); // remove "/n"

        QList<QByteArray> words = line.split(' ');
        QList<QByteArray>::iterator iterator = words.begin();
        QString mode(*iterator);

        if (mode == "v") {
            Vec3d vector;
            for (int i = 0; i < 3; ++i) {
                ++iterator;
                vector[i] = iterator->toDouble();
            }
            vector[1] *= -1;
            object->vertexList.push_back(vector);
        } else if (mode == "vt") {
            Vec2d vector;
            for (int i = 0; i < 2; ++i) {
                ++iterator;

                vector[i] = iterator->toDouble();
            }
            vector[1] = 1 - vector[1];
            object->textureVertexList.push_back(vector);
        } else if (mode == "vn") {
            Vec3d vector;
            for (int i = 0; i < 3; ++i) {
                ++iterator;

                vector[i] = iterator->toDouble();
            }
            vector[1] *= -1;
            object->normalVectors.push_back(vector);
        } else if (mode == "f") {
            Vector<3, Vertex> t;
            for (size_t i = 0; i < 3; ++i) {
                ++iterator;
                Vertex vertex;
                QList<QByteArray> array = iterator->split('/');

                QList<QByteArray>::iterator j = array.begin();

                vertex.setCoordinates(object->vertexList.at((j++)->toInt() - 1));
                vertex.setTextureCoordinates(object->textureVertexList.at((j++)->toInt() - 1));
                vertex.setNormalVector(object->normalVectors.at(j->toInt() - 1));

                t[i] = vertex;
            }
            object->triangles.push_back(t);
        }
    }

    file.close();

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
