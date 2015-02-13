#include <QFile>
#include "warefrontobject.h"

WarefrontObject::WarefrontObject(QString file_path, QObject *parent)
        : QObject(parent)
{
    QFile file(file_path, this);

    if (!file.open(QIODevice::ReadOnly))
        return;

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        QList<QByteArray> words = line.split(' ');

        QList<QByteArray>::iterator iterator = words.begin();
        QString mode(*iterator);

        if (mode == "v") {
            Vec3d vector = {};
            for (int i = 0; i < 3; ++i) {
                ++iterator;
                vector[i] = iterator->toDouble();
            }
            vertexList.push_back(vector);
        }
    }

    file.close();
}
