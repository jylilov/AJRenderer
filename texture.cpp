#include "texture.h"

template <>
Buffer *Buffer::createBuffer(uint width, uint height) {
    QImage img(width, height, QImage::Format_Indexed8);
    return new Buffer(img);
}

template <>
Texture *Texture::fromFile(QString filePath) {
    QImage img(filePath);
    return new Texture(img.convertToFormat(QImage::Format_RGB888));
}
