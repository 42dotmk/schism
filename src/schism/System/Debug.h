#pragma once

#include <assert.h>
#include "Log.h"

#ifdef SC_DEBUG
#define SC_CONST_ASSERT(x, ...)                                     \
    {                                                               \
        if constexpr (!(x)) {                                       \
            SC_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); \
            assert(x);                                              \
        }                                                           \
    }
#define SC_ASSERT(x, ...)                                           \
    {                                                               \
        if (!(x)) {                                                 \
            SC_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); \
            assert(x);                                              \
        }                                                           \
    }
#define SC_ERR(x, ...)                  \
    if (!(x)) {                         \
        SC_CORE_CRITICAL(__VA__ARGS__); \
    }

#else
#define SC_ASSERT(x, ...) \
    {}
#define SC_CONST_ASSERT(x, ...) \
    {}
#define SC_ERR(x, ...) \
    {}
#endif

#define SC_STATIC_FAIL(str) \
    { static_assert(false, str); }
