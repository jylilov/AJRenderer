#include "vector.h"

template <size_t dimCount, typename T>
Vector<dimCount, T> operator+ (Vector<dimCount, T> v1, Vector<dimCount, T> v2) {
    Vector<dimCount, T> answer;
    for (int i = 0; i < dimCount; ++i) {
        answer[i] = v1[i] + v2[i];
    }
}
