#include <QtGui/qvector2d.h>
#include "renderer.h"

Renderer::Renderer(quint32 width, quint32 height)
        : width(width), height(height), image(NULL)
{

}

QPixmap Renderer::render() {
    if (!image) {
        image = new QImage(width, height, QImage::Format_RGB888);
        image->fill(qRgb(0, 0, 0));
    }

    return QPixmap::fromImage(*image);
}