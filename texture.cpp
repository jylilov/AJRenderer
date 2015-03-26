#include "texture.h"

template <>
Buffer *Buffer::createBuffer(uint width, uint height) {
    return new Buffer(width, height);
}

template <>
Texture *Texture::fromFile(QString const &filePath) {
    QFile file(filePath);
    if (file.exists()) {
        QImage img(filePath);
        img = img.convertToFormat(QImage::Format_RGB888);
        return new Texture(img.width(), img.height(), img.bits());
    } else {
        return 0;
    }
}
