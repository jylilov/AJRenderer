#pragma once

#include <cstring>
#include "vector.h"

template <size_t colCount, size_t rowCount, typename T>
class Matrix;

class Determinant {
public:
    template <typename T>
    static T countDeterminant(const Matrix<1, 1, T> &matrix) {
        return matrix[0][0];
    }

    template <size_t dimCount, typename T>
    static T countDeterminant(const Matrix<dimCount, dimCount, T> &matrix) {
        T answer = T();
        for (size_t i = 0; i < dimCount; ++i) {
            answer += matrix.getAlgebraicComplement(i, 0) * matrix[i][0];
        }
        return answer;
    }
};

template <size_t colCount, size_t rowCount, typename T>
class Matrix {
private:
    Vector<rowCount, T> items[colCount];
public:
    static Matrix getIdentityMatrix() {
        assert(colCount == rowCount);
        Matrix m;
        for (int i = 0; i < colCount; ++i) m[i][i] = 1;
        return m;
    }

    static Matrix getScaleMatrix(double scale) {
        Matrix m = getIdentityMatrix();
        for (int i = 0; i < colCount - 1; ++i) m[i][i] *= scale;
        return m;
    }

    Matrix() { for (int i = 0; i < colCount; ++i) items[i] = Vector<rowCount, T>();}
    Matrix(const Matrix &m) { *this = m; }

    Matrix(T *array) {
        size_t k = 0;
        for (size_t i = 0; i < colCount; ++i)
            for (size_t j = 0; j < rowCount; ++j)
                items[i][j] = array[k++];
    }

    Matrix<colCount - 1, rowCount - 1, T> getMinor(size_t x, size_t y) const {
        assert(x <= colCount && y <= rowCount);
        T array[colCount * rowCount];
        size_t k = 0;
        for (size_t i = 0; i < colCount; ++i) {
            for (size_t j = 0; j < rowCount; ++j) {
                if (i != x && j != y)
                    array[k++] = items[i][j];
            }
        }
        return Matrix <colCount - 1, rowCount - 1, T>(array);
    }

    T getAlgebraicComplement(size_t x, size_t y) const {
        assert(rowCount == colCount);
        assert(x <= colCount && y <= rowCount);
        return ((x + y) % 2 ? -1 : 1) * getMinor(x, y).getDeterminant();
    }

    T getDeterminant() {
        return Determinant::countDeterminant(*this);
    }

    Matrix getAdjointMatrix() {
        T array[colCount * rowCount];
        size_t k = 0;
        for (size_t i = 0; i < rowCount; ++i)
            for (size_t j = 0; j < colCount; ++j)
                array[k++] = getAlgebraicComplement(j, i);
        return Matrix<colCount, rowCount, T>(array);
    }

    Matrix getInverseMatrix() {
        Matrix adj = getAdjointMatrix();
        return adj / getDeterminant();
    }

    Vector<rowCount, T>& operator [](size_t index) {
        assert(index < rowCount);
        return items[index];
    }

    const Vector<rowCount, T>& operator [](size_t index) const {
        assert(index < rowCount);
        return items[index];
    }

    Matrix &operator= (const Matrix &m) {
        for (int i = 0; i < rowCount; ++i) {
            this->items[i] = m.items[i];
        }
        return *this;
    }
};

template <size_t colCount, size_t rowCount, typename T>
Matrix<colCount, rowCount, T>operator* (
        const Matrix<colCount, rowCount, T> &matrix,
        const T &value)
{
    Matrix<colCount, rowCount, T> answer;
    for (int i = 0; i < colCount; ++i) {
        answer[i] = matrix[i] * value;
    }
    return answer;
}

template <size_t colCount, size_t rowCount, typename T>
Matrix<colCount, rowCount, T>operator/ (
        const Matrix<colCount, rowCount, T> &matrix,
        const T &value)
{
    return matrix * (1 / value);
}

template <size_t dimCount, typename T>
Vector<dimCount, T> operator* (
        const Matrix<dimCount, dimCount, T> &matrix,
        const Vector<dimCount, T> &vector)
{
    Vector<dimCount, T> answer;
    for (int i = 0; i < dimCount; ++i) {
        answer[i] = 0;
        for (int j = 0; j < dimCount; ++j) {
            answer[i] += vector[j] * matrix[i][j];
        }
    }
    return answer;
}


template <size_t dimCount, typename T>
Matrix<dimCount, dimCount, T> operator* (
        const Matrix<dimCount, dimCount, T> &m1,
        const Matrix<dimCount, dimCount, T> &m2)
{
    Matrix<dimCount, dimCount, T> answer;
    for (int i = 0; i < dimCount; ++i) {
        for (int j = 0; j < dimCount; ++j) {
            for (int k = 0; k < dimCount; ++k) {
                answer[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return answer;
}

typedef Matrix<4, 4, double> Mat4d;
typedef Matrix<3, 3, double> Mat3d;