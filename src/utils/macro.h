#pragma once

#ifdef __AVR__
#define LP_MAKE_CALLBACK(name, type, ...) typedef type (*name)(__VA_ARGS__)
#define LP_CALLBACK(name, type, ...) type (*name)(__VA_ARGS__)
#else
#include <functional>
#define LP_MAKE_CALLBACK(name, type, ...) typedef std::function<type(__VA_ARGS__)> name
#define LP_CALLBACK(name, type, ...) std::function<type(__VA_ARGS__)> name
#endif

#define _LP_CONCAT_IMPL(a, b) a##b
#define _LP_CONCAT(a, b) _LP_CONCAT_IMPL(a, b)