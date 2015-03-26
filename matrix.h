#pragma once

#include <QtCore>
#include <strings.h>
#include "vector.h"

template <uint rowCount, uint colCount, typename T>
class Matrix;

class Determinant {
public:
    template <typename T>
    static T countDeterminant(Matrix<1, 1, T> const &matrix) {
        return matrix[0][0];
    }

    template <uint dimCount, typename T>
    static T countDeterminant(Matrix<dimCount, dimCount, T> const &matrix) {
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
    static Matrix getScaleMatrix(double scale) {
        Matrix m = getIdentityMatrix();
        for (uint i = 0; i < rowCount; ++i) m[i][i] *= scale;
        return m;
    }
    template <uint dimCount>
    static Matrix getScaleMatrix(Vector<dimCount, double> vector) {
        Matrix m = getIdentityMatrix();
        for (uint i = 0; i < qMin(rowCount, dimCount); ++i) {
            m[i][i] *= vector[i];
        }
        return m;
    }
private:
    Vector<colCount, T> items[rowCount];
public:
    Matrix() {}

    Matrix(Matrix const &m) { *this = m; }

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

    Vector<rowCount, T> getColumn(uint index) const {
        Vector<rowCount, T> vector;
        for (uint i = 0; i < rowCount; ++i) {
            vector[i] = items[i][index];
        }
        return vector;
    }

    void setColumn(uint index, Vector<rowCount, T> vector) {
        for (uint i = 0; i < rowCount; ++i) {
            items[i][index] = vector[i];
        }
    }

    Matrix<colCount, rowCount, T> getTransposeMatrix() const {
        Matrix<colCount, rowCount, T> m;
        for (uint i = 0; i < rowCount; ++i)
            for (uint j = 0; j < colCount; ++j)
                m[j][i] = items[i][j];
        return m;
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

    T getDeterminant() const {
        return Determinant::countDeterminant(*this);
    }

    Matrix<colCount, rowCount, T> getAdjointMatrix() const {
        Matrix<colCount, rowCount, T> answer;
        for (uint i = 0; i < colCount; ++i)
            for (uint j = 0; j < rowCount; ++j)
                answer[i][j] = getAlgebraicComplement(j, i);
        return answer;
    }

    Matrix getInverseMatrix() const {
        return getAdjointMatrix() / getDeterminant();
    }

    Matrix &operator *= (T const &value) {
        for (uint i = 0; i < rowCount; ++i) {
            items[i] *= value;
        }
        return *this;
    }

    Matrix &operator /= (T const &value) {
        return *this *= (1 / value);
    }

    Vector<colCount, T> &operator [] (uint index) {
        Q_ASSERT(index < rowCount);
        return items[index];
    }

    const Vector<colCount, T> &operator [] (uint index) const {
        Q_ASSERT(index < rowCount);
        return items[index];
    }

    Matrix &operator = (Matrix const &m) {
        for (uint i = 0; i < colCount; ++i) {
            this->items[i] = m.items[i];
        }
        return *this;
    }
};

template <uint rowCount, uint colCount, typename T>
Matrix<rowCount, colCount, T> operator * (
        Matrix<rowCount, colCount, T> const &matrix,
        T const &value)
{
    Matrix<rowCount, colCount, T> answer = matrix;
    return answer *= value;
}

template <uint rowCount, uint colCount, typename T>
Matrix<rowCount, colCount, T> operator / (
        Matrix<rowCount, colCount, T> const &matrix,
        T const &value)
{
    return matrix * (1 / value);
}

template <uint colCount, uint rowCount, typename T>
Vector<rowCount, T> operator* (
        Matrix<rowCount, colCount, T> const &matrix,
        Vector<colCount, T> const &vector)
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
Matrix<dim1, dim3, T> operator * (
        Matrix<dim1, dim2, T> const &m1,
        Matrix<dim2, dim3, T> const &m2)
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