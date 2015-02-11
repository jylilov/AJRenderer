#include <cassert>
#include <cstring>

template <size_t dimCount, typename T>
struct Vector {
    T items[dimCount];

    T& operator [](size_t index) {
        assert(index < dimCount);
        return items[index];
    }

    const T& operator [](size_t index) const {
        assert(index < dimCount);
        return items[index];
    }
};

typedef Vector<2, int> Vec2i;