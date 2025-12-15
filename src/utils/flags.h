#pragma once
#include <inttypes.h>
#include <stddef.h>

#if defined(__GNUC__)
#define _LP_INLINE __attribute__((always_inline)) inline
#else
#define _LP_INLINE inline
#endif

namespace looper {

struct Flags {
    // пакет флагов
    uint8_t flags = 0;

    // установить маску
    _LP_INLINE void set(uint8_t mask) {
        flags |= mask;
    }

    // очистить маску
    _LP_INLINE void clear(uint8_t mask) {
        flags &= uint8_t(~mask);
    }

    // записать маску
    _LP_INLINE void write(uint8_t mask, bool val) {
        val ? set(mask) : clear(mask);
    }

    // записать биты по маске
    _LP_INLINE void writeBits(uint8_t mask, uint8_t bits) {
        flags = (flags & uint8_t(~mask)) | (bits & mask);
    }

    // прочитать маску
    _LP_INLINE uint8_t read(uint8_t mask) const {
        return flags & mask;
    }

    // стоят все биты в маске
    _LP_INLINE bool isSet(uint8_t mask) const {
        return read(mask) == mask;
    }

    // очищены все биты в маске
    _LP_INLINE bool isClear(uint8_t mask) const {
        return read(mask) == 0;
    }

    // сравнить маску со значением
    _LP_INLINE bool compare(uint8_t mask, uint8_t val) const {
        return read(mask) == val;
    }
};

}  // namespace looper