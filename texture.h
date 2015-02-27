#include <QImage>
#include <QtCore>

class Texture {
private:
    QImage image;
public:
    Texture(QString filePath) : image(filePath) {}

    uint getColor(double x, double y) const {
        return image.pixel(
                qRound(x * image.width()),
                qRound(y * image.height())
        );
    }

};