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
            object->vertexList.push_back(vector);
        } else if (mode == "f") {
            Triangle triangle;
            for (size_t i = 0; i < 3; ++i) {
                ++iterator;
                QList<QByteArray> array = iterator->split('/');

                QList<QByteArray>::iterator j = array.begin();

                triangle.setVertex(i, object->vertexList.at((j++)->toInt() - 1));
                //TODO Process other parameters
            }
            object->trianglesList.push_back(triangle);
        }
    }

    file.close();

    return object;
}
