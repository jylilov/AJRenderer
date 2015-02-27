#include "triangle.h"

Vec4d Triangle::getVertex(size_t index) const {
    switch (index) {
        case 0:
            return getVertex1();
        case 1:
            return getVertex2();
        case 2:
        default:
            return getVertex3();
    }
}

Vec2i Triangle::getTextureVertex(size_t index) const {
    switch (index) {
        case 0:
            return getTextureVertex1();
        case 1:
            return getTextureVertex2();
        case 2:
        default:
            return getTextureVertex3();
    }
}

void Triangle::setVertex(size_t index, Vec4d vertex) {
    switch (index) {
        case 0:
            setVertex1(vertex);
            break;
        case 1:
            setVertex2(vertex);
            break;
        case 2:
        default:
            setVertex3(vertex);
            break;
    }
}

void Triangle::setTextureVertex(size_t index, Vec2i vertex) {
    switch (index) {
        case 0:
            setTextureVertex1(vertex);
            break;
        case 1:
            setTextureVertex2(vertex);
            break;
        case 2:
        default:
            setTextureVertex3(vertex);
            break;
    }
}
