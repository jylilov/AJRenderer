#pragma once

#include <QtCore>
#include "vector.h"

template <uint rowCount, uint colCount, typename T>
class Matrix;

class Determinant {
public:
    template <typename T>
    static T countDeterminant(const Matrix<1, 1, T> &matrix) {
        return matrix[0][0];
    }

    template <uint dimCount, typename T>
    static T countDeterminant(const Matrix<dimCount, dimCount, T> &matrix) {
        T answer = T();
        for (uint i = 0; i < dimCount; ++i) {
            answer += matrix.getAlgebraicComplement(i, 0) * matrix[i][0];
        }
        return answer;
    }
};

template <uint rowCount, uint colCount, typename T>
class Matrix {
public:
    static Matrix getIdentityMatrix() {
        Q_ASSERT(rowCount == colCount);
        Matrix m;
        for (uint i = 0; i < rowCount; ++i) m[i][i] = 1;
        return m;
    }
//TODO more beautiful
    static Matrix getScaleMatrix(double scale) {
        Matrix m = getIdentityMatrix();
        for (uint i = 0; i < rowCount - 1; ++i) m[i][i] *= scale;
        return m;
    }
private:
    Vector<colCount, T> items[rowCount];
public:
    Matrix() {}

    Matrix(const Matrix &m) { *this = m; }

    Matrix(T first, ...) {
        items[0][0] = first;

        va_list list;
        va_start(list, first);

        for (uint i = 0; i < rowCount; ++i) {
            for (uint j = (i != 0) ? 0 : 1; j < colCount; ++j) {
                items[i][j] = va_arg(list, T);
            }
        }
        va_end(list);
    }

    Matrix<rowCount - 1, colCount - 1, T> getMinor(uint x, uint y) const {
        Q_ASSERT(x <= rowCount && y <= colCount);
        Matrix<rowCount - 1, colCount - 1, T> answer;
        for (uint i = 0; i < rowCount - 1; ++i) {
            for (uint j = 0; j < colCount - 1; ++j) {
                answer[i][j] = items[i < x ? i : i + 1][j < y ? j : j + 1];
            }
        }
        return answer;
    }

    T getAlgebraicComplement(uint x, uint y) const {
        Q_ASSERT(colCount == rowCount);
        Q_ASSERT(x <= rowCount && y <= colCount);
        return ((x + y) % 2 ? -1 : 1) * getMinor(x, y).getDeterminant();
    }

    T getDeterminant() {
        return Determinant::countDeterminant(*this);
    }

    Matrix<colCount, rowCount, T> getAdjointMatrix() {
        Matrix<colCount, rowCount, T> answer;
        for (uint i = 0; i < colCount; ++i)
            for (uint j = 0; j < rowCount; ++j)
                answer[i][j] = getAlgebraicComplement(j, i);
        return answer;
    }

    Matrix getInverseMatrix() {
        return getAdjointMatrix() / getDeterminant();
    }

    Matrix &operator *= (const T &value) {
        for (uint i = 0; i < rowCount; ++i) {
            items[i] *= value;
        }
        return *this;
    }

    Matrix &operator /= (const T &value) {
        return *this *= (1 / value);
    }

    Vector<colCount, T> &operator [] (uint index) {
        Q_ASSERT(index < colCount);
        return items[index];
    }

    const Vector<colCount, T> &operator [] (uint index) const {
        Q_ASSERT(index < colCount);
        return items[index];
    }

    Matrix &operator = (const Matrix &m) {
        for (uint i = 0; i < colCount; ++i) {
            this->items[i] = m.items[i];
        }
        return *this;
    }
};

template <uint rowCount, uint colCount, typename T>
Matrix<rowCount, colCount, T> operator * (
        const Matrix<rowCount, colCount, T> &matrix,
        const T &value)
{
    Matrix<rowCount, colCount, T> answer = matrix;
    return answer *= value;
}

template <uint rowCount, uint colCount, typename T>
Matrix<rowCount, colCount, T> operator / (
        const Matrix<rowCount, colCount, T> &matrix,
        const T &value)
{
    return matrix * (1 / value);
}

template <uint colCount, uint rowCount, typename T>
Vector<rowCount, T> operator* (
        const Matrix<rowCount, colCount, T> &matrix,
        const Vector<colCount, T> &vector)
{
    Vector<rowCount, T> answer;
    for (int i = 0; i < rowCount; ++i) {
        answer[i] = 0;
        for (int j = 0; j < colCount; ++j) {
            answer[i] += vector[j] * matrix[i][j];
        }
    }
    return answer;
}


template <uint dim1, uint dim2, uint dim3, typename T>
Matrix<dim1, dim3, T> operator* (
        const Matrix<dim1, dim2, T> &m1,
        const Matrix<dim2, dim3, T> &m2)
{
    Matrix<dim1, dim3, T> answer;
    for (int i = 0; i < dim1; ++i) {
        for (int j = 0; j < dim3; ++j) {
            for (int k = 0; k < dim2; ++k) {
                answer[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return answer;
}

typedef Matrix<4, 4, double> Mat4d;
typedef Matrix<3, 3, double> Mat3d;