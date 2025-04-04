# pragma once

#include "macros.h"

#define LOG_WARN_ENABLED 1
#define LOG_INFO_ENABLED 1
#define LOG_DEBUG_ENABLED 1
#define LOG_TRACE_ENABLED 1

// DISABLE DEUBG AND TRACE LOGGING FOR RELEASE BUILDS
#if CRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

typedef enum CODI_LogLevel {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} CODI_LogLevel;

b8 initializeLogging();
void shutdownLogging();

CAPI void CODI_LogOutput(const CODI_LogLevel lvl, const char* msg, ...);

#ifndef CFATAL
// Logs a fatal-level message.
#define CFATAL(msg, ...) CODI_LogOutput(LOG_LEVEL_FATAL, msg, ##__VA_ARGS__)
#endif

#ifndef CERROR
// Logs an error-level message.
#define CERROR(msg, ...) CODI_LogOutput(LOG_LEVEL_ERROR, msg, ##__VA_ARGS__)
#endif

#if LOG_WARN_ENABLED == 1
// Logs an error-level message.
#define CWARN(msg, ...) CODI_LogOutput(LOG_LEVEL_WARN, msg, ##__VA_ARGS__)
#else
#define CWARN(msg, ...)
#endif

#if LOG_INFO_ENABLED == 1
// Logs an error-level message.
#define CINFO(msg, ...) CODI_LogOutput(LOG_LEVEL_INFO, msg, ##__VA_ARGS__)
#else
#define CINFO(msg, ...)
#endif

#if LOG_DEBUG_ENABLED == 1
// Logs an error-level message.
#define CDEBUG(msg, ...) CODI_LogOutput(LOG_LEVEL_DEBUG, msg, ##__VA_ARGS__)
#else
#define CDEBUG(msg, ...)
#endif

#if LOG_TRACE_ENABLED == 1
// Logs an error-level message.
#define CTRACE(msg, ...) CODI_LogOutput(LOG_LEVEL_TRACE, msg, ##__VA_ARGS__)
#else
#define CTRACE(msg, ...)
#endif
