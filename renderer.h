#include <QtCore/qglobal.h>
#include <QtGui/qpixmap.h>
#include "vector.h"

class Renderer
{
private:
    quint32 width;
    quint32 height;

    QImage *image;

public:
    Renderer(quint32 width, quint32 height);

    QPixmap render();

private:
    void renderLine(Vec2i p0, Vec2i p1, uint color);


};