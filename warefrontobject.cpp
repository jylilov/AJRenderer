#include "warefrontobject.h"

WarefrontObject::WarefrontObject(QString file_path)
        : coordinates(), size(0)
{
    QFile file(file_path);

    if (!file.open(QIODevice::ReadOnly))
        return;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
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
            vertexList.push_back(vector);
        } else if (mode == "f") {
            Vec3i vector;
            for (int i = 0; i < 3; ++i) {
                ++iterator;
                //TODO Process other parameters
                QList<QByteArray> array = iterator->split('/');
                vector[i] = array.begin()->toInt() - 1;
            }
            trianglesList.push_back(vector);
        }
    }

    file.close();
}
