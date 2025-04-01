#include "logger.h"
#include "asserts.h"
#include "platform/platform.h"

// TODO: temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

b8 initializeLogging() {
    // TODO: create a log file
    return TRUE;
}

void shutdownLogging() {
    // TODO: cleanup logging/write queued entries
}

void logOutput(const logLevel lvl, const char* msg, ...) {
    const char* levelStrings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]: ", "[INFO]: ", "[DEBUG]: ", "[TRACE]: "};
    b8 isError = lvl < LOG_LEVEL_WARN;

    // NOTE: this is not cool thingy to do !! needs to be dynamic 
    const i32 msgLength = 32000;
    char outMsg[msgLength];
    memset(outMsg, 0, sizeof(outMsg));

    // format original message
    __builtin_va_list argPtr;
    va_start(argPtr, msg);
    vsnprintf(outMsg, msgLength, msg, argPtr);
    va_end(argPtr);

    char finalMsg[msgLength];
    sprintf(finalMsg, "%s%s\n", levelStrings[lvl], outMsg);

    if (isError) platform_ConsoleWriteError(finalMsg, lvl);
    else platform_ConsoleWrite(finalMsg, lvl);
}

void reportAssertionFailure(const char* expr, const char* msg, const char* file, i32 line) {
    logOutput(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expr, msg, file, line);
}
