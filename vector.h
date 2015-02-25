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

typedef Vector<2, int> Vec2i;
typedef Vector<4, double> Vec4d;
typedef Vector<3, int> Vec3i;

template <size_t dimCount, typename T>
Vector<dimCount, T> operator+ (Vector<dimCount, T> v1, Vector<dimCount, T> v2);