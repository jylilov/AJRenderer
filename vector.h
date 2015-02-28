#pragma once

#include <QtCore>

template <uint dimCount, typename T>
class Vector {
public:
    template <uint oldDimCount>
    static Vector cast(const Vector<oldDimCount, T> &vector) {
        Vector v;
        for (uint i = 0; i < qMin(oldDimCount, dimCount); ++i) {
            v[i] = vector[i];
        }
        return v;
    }

    static Vector<dimCount, int> cast(const Vector<dimCount, double> &vector) {
        Vector<dimCount, int> v;
        for (uint i = 0; i < dimCount; ++i) {
            v[i] = qRound(vector[i]);
        }
        return v;
    }

    template <typename newT>
    static Vector cast(const Vector<dimCount, newT> &vector) {
        Vector v;
        for (uint i = 0; i < dimCount; ++i) {
            v[i] = vector[i];
        }
        return v;
    }
private:
    T items[dimCount];
public:
    Vector() { for (uint i = 0; i < dimCount; ++i) items[i] = T(); }
    Vector(const Vector &v) { *this = v; }

    Vector(T first, ...) {
        items[0] = first;

        va_list list;
        va_start(list, first);

        for (uint i = 1; i < dimCount; ++i) {
            items[i] = va_arg(list, T);
        }

        va_end(list);
    }

    T getLength() {
        T answer = T();
        for (uint i = 0; i < dimCount; ++i) {
            answer += items[i] * items[i];
        }
        return qSqrt(answer);
    }

    void normalize() {
        *this = *this / getLength();
    }

    Vector &operator += (const Vector &vector) {
        for (uint i = 0; i < dimCount; ++i)
            items[i] += vector[i];
        return *this;
    }

    Vector &operator *= (const T &value) {
        for (uint i = 0; i < dimCount; ++i)
            items[i] *= value;
        return *this;
    }

    Vector &operator /= (const T &value) {
        return *this *= (1 / value);
    }

    T &operator [] (uint index) {
        Q_ASSERT(index < dimCount);
        return items[index];
    }

    const T &operator [] (uint index) const {
        Q_ASSERT(index < dimCount);
        return items[index];
    }

    Vector &operator = (const Vector &v) {
        for (int i = 0; i < dimCount; ++i) {
            this->items[i] = v.items[i];
        }
        return *this;
    }
};

template <uint dimCount, typename T>
Vector<dimCount, T> operator + (const Vector<dimCount, T> &v1, const Vector<dimCount, T> &v2) {
    Vector<dimCount, T> answer = v1;
    return answer += v2;
}

template <uint dimCount, typename T>
Vector<dimCount, T> operator * (const Vector<dimCount, T> &vertex, const T &value) {
    Vector<dimCount, T> answer = vertex;
    return answer *= value;
}

template <uint dimCount, typename T>
T operator * (const Vector<dimCount, T> &v1, const Vector<dimCount, T> &v2) {
    T answer = T();
    for (int i = 0; i < dimCount; ++i) {
        answer += v1[i] * v2[i];
    }
    return answer;
}

template <uint dimCount, typename T>
Vector<dimCount, T> operator / (const Vector<dimCount, T> &v1, const T &value) {
    return v1 * (1 / value);
}

typedef Vector<3, int> Vec3i;
typedef Vector<4, double> Vec4d;
typedef Vector<3, double> Vec3d;
typedef Vector<2, double> Vec2d;

