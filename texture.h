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
//    QImage image;
    uchar *data;
    uint width;
    uint height;
    uint dataSize;

    Image(uint width, uint height) : width(width), height(height), dataSize(width * height * componentCount) {
        data = new uchar[dataSize];
    }
    Image(uint width, uint height, uchar *data) {
        Image(width, height);
        memcpy(this->data, data, dataSize);
    }
public:
    ~Image() { delete [] data; }
    void fill(uchar channel) {
        memset(data, channel, dataSize);
    }

    Vector<componentCount, uchar> get(Vec2d vector) const {
        int x = qRound(vector[0] * width);
        int y = qRound(vector[1] * height);
        return get(x, y);
    }

    Vector<componentCount, uchar> get(int x, int y) const {
        Vector<componentCount, uchar> ans;
        if (!(x > 0 && y > 0 && x < width && y < height))
            return ans;
        for (uint i = 0; i < componentCount; ++i) {
            ans[i] = data[(x + y * height) * componentCount + i];
        }
        return ans;
    }

    void set(int x, int y, uchar const &value) {
        for (uint i = 0; i < componentCount; ++i) {
            data[(x  + y * height) * componentCount + i] = value;
        }
    }

};