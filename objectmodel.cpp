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
            Vec4d vector;
            for (int i = 0; i < 3; ++i) {
                ++iterator;
                vector[i] = iterator->toDouble();
            }
            vector[1] *= -1;
            vector[3] = 1;
            object->vertexList.push_back(vector);
        } else if (mode == "vt") {
            Vec3d vector;
            for (int i = 0; i < 2; ++i) {
                ++iterator;

                vector[i] = iterator->toDouble();
            }
            vector[1] = 1 - vector[1];
            object->textureVertexList.push_back(vector);
        } else if (mode == "f") {
            Triangle triangle;
            for (size_t i = 0; i < 3; ++i) {
                ++iterator;
                QList<QByteArray> array = iterator->split('/');

                QList<QByteArray>::iterator j = array.begin();

                triangle.setVertex(i, object->vertexList.at((j++)->toInt() - 1));

                triangle.setTextureVertex(i, object->textureVertexList.at((j++)->toInt() - 1));
                //TODO Process normal vectors
            }
            object->trianglesList.push_back(triangle);
        }
    }

    file.close();

    return object;
}

void ObjectModel::updateMatrix() {
    double x[] = {
            1, 0, 0, 0,
            0, qCos(direction[0]), -qSin(direction[0]), 0,
            0, qSin(direction[0]), qCos(direction[0]), 0,
            0, 0, 0, 1
    };
    double y[] = {
            qCos(direction[1]), 0, qSin(direction[1]), 0,
            0, 1, 0, 0,
            -qSin(direction[1]), 0, qCos(direction[1]), 0,
            0, 0, 0, 1
    };
    double z[] = {
            qCos(direction[2]), -qSin(direction[2]), 0, 0,
            qSin(direction[2]), qCos(direction[2]), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    };

    modelMatrix = Mat4d::getScaleMatrix(size / 2) * Mat4d(x) * Mat4d(y) * Mat4d(z);

}
