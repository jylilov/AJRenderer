#pragma once

#include <cassert>
#include <cstring>

template <size_t dimCount, typename T>
class Vector {
private:
    T items[dimCount];
public:
    Vector() {}
    Vector(const Vector &v) { *this = v; }

    T &operator[] (size_t index) {
        assert(index < dimCount);
        return items[index];
    }

    const T &operator[] (size_t index) const {
        assert(index < dimCount);
        return items[index];
    }

    Vector &operator= (const Vector &v) {
        for (int i = 0; i < dimCount; ++i) {
            this->items[i] = v.items[i];
        }
        return *this;
    }
};

template <size_t dimCount, typename T>
Vector<dimCount, T> operator+ (const Vector<dimCount, T> &v1, const Vector<dimCount, T> &v2) {
    Vector<dimCount, T> answer;
    for (int i = 0; i < dimCount; ++i) {
        answer[i] = v1[i] + v2[i];
    }
}

template <size_t dimCount, typename T>
Vector<dimCount, T> operator* (const Vector<dimCount, T> &v1, const double &value) {
    Vector<dimCount, T> answer;
    for (int i = 0; i < dimCount; ++i) {
        answer[i] = v1[i] * value;
    }
    return answer;
}


typedef Vector<3, int> Vec3i;
typedef Vector<2, int> Vec2i;
typedef Vector<4, double> Vec4d;
typedef Vector<3, double> Vec3d;
typedef Vector<2, double> Vec2d;

