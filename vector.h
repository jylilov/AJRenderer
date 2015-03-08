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
            v[i] = static_cast<newT>(vector[i]);
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

    T getLength() const {
        T answer = T();
        for (uint i = 0; i < dimCount; ++i) {
            answer += items[i] * items[i];
        }
        return qSqrt(answer);
    }

    Vector getNormalVector() const {
        return *this / getLength();
    }

    Vector<dimCount - 1, T> getProjection() const {
        T del = items[dimCount - 1];
        if (del == 0)
            return Vector<dimCount - 1, T>::cast(*this);
        else
            return Vector<dimCount - 1, T>::cast(*this / del);
    }

    Vector<dimCount + 1, T> getExtention(T fill = 1) const {
        Vector<dimCount + 1, T> ans = Vector<dimCount + 1, T>::cast(*this);
        ans[dimCount] = fill;
        return ans;
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

    Vector operator - () const {
        return *this * T(-1);
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
Vector<dimCount, T> operator - (const Vector<dimCount, T> &v1, const Vector<dimCount, T> &v2) {
    Vector<dimCount, T> answer = v1;
    return answer += -v2;
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

template <typename T>
Vector<3, T> cross(Vector<3, T> v1, Vector<3, T> v2) {
    return Vector<3,T>(
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]);
}

typedef Vector<3, int> Vec3i;
typedef Vector<3, uchar> VecColor;
typedef Vector<4, double> Vec4d;
typedef Vector<3, double> Vec3d;
typedef Vector<2, double> Vec2d;

