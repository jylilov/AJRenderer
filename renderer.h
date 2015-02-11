#include <QtCore/qglobal.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qopengles2ext.h>

class Renderer
{
public:
    Renderer(quint32 width, quint32 height);

    QPixmap render();

private:
    quint32 width;
    quint32 height;

    QImage *image;
};