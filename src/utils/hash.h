#pragma once
#include <inttypes.h>
#include <stddef.h>

typedef size_t hash_t;

// хэш на этапе компиляции
static constexpr hash_t LPH(const char* str, const hash_t h = 0) {
    return (*str ? LPH(str + 1, h + (h << 5) + *str) : h);
}
static constexpr hash_t LPH(hash_t hash) {
    return hash;
}

// хэш для рантайма
inline hash_t LPHr(const char* str) {
    hash_t hash = 0;
    while (*str) hash = hash + (hash << 5) + *str++;
    return hash;
}
inline hash_t LPHr(hash_t hash) {
    return hash;
}