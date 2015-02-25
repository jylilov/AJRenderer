#pragma once

#include <cstring>
#include "vector.h"

template <size_t dimCount, typename T>
class Matrix {
    Matrix() {}
    Matrix(Matrix &m) { *this = m; }

    Vector<dimCount, T> items[dimCount];

    Vector<dimCount, T>& operator [](size_t index) {
        assert(index < dimCount);
        return items[index];
    }

    const Vector<dimCount, T>& operator [](size_t index) const {
        assert(index < dimCount);
        return items[index];
    }

    Matrix &operator= (const Matrix &m) {
        for (int i = 0; i < dimCount; ++i) {
            this->items[i] = m.items[i];
        }
    }
};

typedef Matrix<4, double> Mat4d;