#pragma once

#include <QImage>
#include <QtCore>
#include "vector.h"

template <uint componentCount>
class Image;

typedef Image<1> Buffer;
typedef Image<3> Texture;

template <uint componentCount>
class Image {
public:
    static Texture *fromFile(QString filePath);
    static Buffer *createBuffer(uint width, uint height);
private:
    QImage image;

    Image() {}
    Image(QImage img) : image(img) {}
public:
    void save() {
        QImage img(image.width(), image.height(), QImage::Format_RGB888);
        for (int i = 0; i < img.width(); ++i) {
            for (int j = 0; j < img.height(); ++j) {
                uchar c = get(i, j)[0];
                img.setPixel(i, j, qRgb(c, c, c));
            }
        }
        img.save("/home/jylilov/TEMP/temp.png");
    }

    void fill(uchar channel) {
        uint size = image.width() * image.height() * componentCount;
        uchar *bits = image.bits();

        for (uint i = 0; i < size; ++i)
            bits[i] = channel;
    }

    Vector<componentCount, uchar> get(Vec2d vector) const {
        int x = qRound(vector[0] * image.width());
        int y = qRound(vector[1] * image.height());
        return get(x, y);
    }

    Vector<componentCount, uchar> get(int x, int y) const {

        Vector<componentCount, uchar> ans;
        if (!(x > 0 && y > 0 && x < image.width() && y < image.height()))
            return ans;
        for (uint i = 0; i < componentCount; ++i) {
            ans[i] = image.bits()[(x  + y * image.height()) * componentCount + i];
        }
        return ans;
    }

    void set(int x, int y, uchar const &value) {
        for (uint i = 0; i < componentCount; ++i) {
            image.bits()[(x  + y * image.height()) * componentCount + i] = value;
        }
    }

};