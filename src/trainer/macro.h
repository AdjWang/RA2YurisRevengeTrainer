#pragma once

#ifndef DISALLOW_COPY
#define DISALLOW_COPY(T) T(const T& other) = delete
#endif

#ifndef DISALLOW_ASSIGN
#define DISALLOW_ASSIGN(T) T operator=(const T& other) = delete
#endif

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(T) \
    DISALLOW_COPY(T); \
    DISALLOW_ASSIGN(T)
#endif

#ifndef UNREACHABLE
// https://en.cppreference.com/w/cpp/utility/unreachable
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
    #define UNREACHABLE() __assume(false)
#else // GCC, Clang
    #define UNREACHABLE() __builtin_unreachable()
#endif
#endif

// #define LIKELY(cond)    __builtin_expect (!!(cond), 1)
// #define UNLIKELY(cond)  __builtin_expect (!!(cond), 0)
#define LIKELY(cond)    (cond)
#define UNLIKELY(cond)  (cond)
