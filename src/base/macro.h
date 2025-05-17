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
#ifdef _WIN32
#define UNREACHABLE() LOG(FATAL) << "unreachable"; __assume(false)
#elif __linux__
#define UNREACHABLE() LOG(FATAL) << "unreachable"; __builtin_unreachable()
#else
#error unknown platform
#endif
#endif

#ifndef NOT_IMPLEMENTED
#define NOT_IMPLEMENTED() LOG(FATAL) << "not implemented"; abort()
#endif

#define LIKELY(cond)    __builtin_expect (!!(cond), 1)
#define UNLIKELY(cond)  __builtin_expect (!!(cond), 0)

// Unfortunately, msvc not have this.
#ifndef _WIN32
// For clang.
#if __has_attribute(guarded_by)
#else
#warning "No attribute to check mutex"
#endif
#endif

#ifndef UNUSED_VAR
#ifdef _MSC_VER
#define UNUSED_VAR(x) (x)
#else
#define UNUSED_VAR(x)
#endif
#endif

#ifdef _MSC_VER
// From io.h
#define WRITE   _write
// From unistd.h
/* Standard file descriptors.  */
#define	STDIN_FILENO	0	/* Standard input.  */
#define	STDOUT_FILENO	1	/* Standard output.  */
#define	STDERR_FILENO	2	/* Standard error output.  */
#elif __linux__
// From unistd.h
#define WRITE   write
#else
#error unknown platform
#endif

#define CACHE_LINE_SIZE 64

#ifdef _MSC_VER
#define __YRTR_BEGIN_THIRD_PARTY_HEADERS \
    _Pragma("warning(push, 0)") \
    _Pragma("warning(disable:4100)") \
    _Pragma("warning(disable:4324)") \
    _Pragma("warning(disable:4996)")
#define __YRTR_END_THIRD_PARTY_HEADERS _Pragma("warning(pop)")
#else
#define __YRTR_BEGIN_THIRD_PARTY_HEADERS
#define __YRTR_END_THIRD_PARTY_HEADERS
#endif
