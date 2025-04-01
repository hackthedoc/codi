#pragma once

#include "macros.h"

// disable assertions by commenting out the line below
#define CASSERTIONS_ENABLED

#ifdef CASSERTIONS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

CAPI void reportAssertionFailure(const char* expr, const char* msg, const char* file, i32 line);

#define CASSERT(expr)                                               \
    {                                                               \
        if (expr) {                                                 \
        } else {                                                    \
            reportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
            debugBreak();                                           \
        }                                                           \
    }

#define CASSERT_MSG(expr, msg)                                  \
{                                                               \
    if (expr) {                                                 \
    } else {                                                    \
        reportAssertionFailure(#expr, msg, __FILE__, __LINE__); \
        debugBreak();                                           \
    }                                                           \
}

#ifdef _DEBUG
#define CASSERT_DEBUG(expr)                                         \
    {                                                               \
        if (expr) {                                                 \
        } else {                                                    \
            reportAssertionFailure(#expr, "", __FILE__, __LINE__);  \
            debugBreak();                                           \
        }                                                           \
    }
#else
#define CASSERT_DEBUG(expr)
#endif

#else
#define CASSERT(expr)
#define CASSERT_MSG(expr, msg)
#define CASSERT_DEBUG(expr)
#endif
