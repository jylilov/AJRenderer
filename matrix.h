#pragma once

#include <cstring>
#include "vector.h"

template <size_t rowCount, size_t colCount, typename T>
class Matrix;

template <size_t rowCount, size_t colCount, typename T>
class Determinant {
public:
    static T countDeterminant(const Matrix<rowCount, colCount, T> &matrix) {
        if (rowCount == 1) {
            return matrix[0][0];
        } else {
            T answer;
            for (size_t i = 0; i < rowCount; ++i) {
                answer += matrix.getAlgebraicComplement(i, 0);
            }
            return answer;
        }
    }
};

template <size_t rowCount, size_t colCount, typename T>
class Matrix {
private:
    Vector<colCount, T> items[rowCount];
public:
    Matrix() {}
    Matrix(const Matrix &m) { *this = m; }

    Matrix(T *array) {
        size_t k = 0;
        for (size_t i = 0; i < rowCount; ++i)
            for (size_t j = 0; j < colCount; ++j)
                items[i][j] = array[k++];
    }

    Matrix<rowCount - 1, colCount - 1, T> getMinor(size_t x, size_t y) const {
        assert(x <= rowCount && y <= colCount);
        T array[rowCount * colCount];
        size_t k = 0;
        for (size_t i = 0; i < rowCount; ++i) {
            for (size_t j = 0; j < colCount; ++j) {
                if (i != x && j != y)
                    array[k++] = items[i][j];
            }
        }
        Matrix<rowCount - 1, colCount - 1, T> matrix;
        //return Matrix <rowCount - 1, colCount - 1, T>(array);
        return matrix;
    }

    T getAlgebraicComplement(size_t x, size_t y) const {
        assert(colCount == rowCount);
        assert(x <= rowCount && y <= colCount);
        return ((x + y) % 2 ? -1 : 1) * getMinor(x, y).getDeterminant();
    }

    T getDeterminant() const {
        return Determinant<colCount, rowCount, T>::countDeterminant(*this);
    }

    Vector<colCount, T>& operator [](size_t index) {
        assert(index < colCount);
        return items[index];
    }

    const Vector<colCount, T>& operator [](size_t index) const {
        assert(index < colCount);
        return items[index];
    }

    Matrix &operator= (const Matrix &m) {
        for (int i = 0; i < colCount; ++i) {
            this->items[i] = m.items[i];
        }
        return *this;
    }
};

typedef Matrix<4, 4, double> Mat4d;