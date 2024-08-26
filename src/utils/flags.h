#pragma once
#include <inttypes.h>
#include <stddef.h>

namespace looper {

struct Flags {
    // пакет флагов
    uint8_t flags = 0;

    // получить маску
    inline uint8_t mask(const uint8_t x) const __attribute__((always_inline)) {
        return flags & x;
    }

    // прочитать
    inline bool read(const uint8_t x) const __attribute__((always_inline)) {
        return flags & x;
    }

    // установить биты
    inline void set(const uint8_t x) __attribute__((always_inline)) {
        flags |= x;
    }

    // очистить биты
    inline void clear(const uint8_t x) __attribute__((always_inline)) {
        flags &= ~x;
    }

    // записать
    inline void write(const uint8_t x, const bool v) __attribute__((always_inline)) {
        v ? set(x) : clear(x);
    }

    // стоят все биты в маске
    inline bool isSet(const uint8_t x) const __attribute__((always_inline)) {
        return (flags & x) == x;
    }

    // очищены все биты в маске
    inline bool isClear(const uint8_t x) const __attribute__((always_inline)) {
        return !(flags & x);
    }

    // сравнить маску со значением
    inline bool compare(const uint8_t x, const uint8_t y) const __attribute__((always_inline)) {
        return (flags & x) == y;
    }
};

}  // namespace looper