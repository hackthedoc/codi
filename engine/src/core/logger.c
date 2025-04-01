#include "logger.h"
#include "asserts.h"

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
    //b8 isError = lvl < 2;

    // NOTE: this is not cool thingy to do !! needs to be dynamic 
    char outMsg[32000];
    memset(outMsg, 0, sizeof(outMsg));

    // format original message
    __builtin_va_list argPtr;
    va_start(argPtr, msg);
    vsnprintf(outMsg, 32000, msg, argPtr);
    va_end(argPtr);

    char finalMsg[32000];
    sprintf(finalMsg, "%s%s\n", levelStrings[lvl], outMsg);

    // TODO: platform specific output
    printf("%s", finalMsg);
}

void reportAssertionFailure(const char* expr, const char* msg, const char* file, i32 line) {
    logOutput(LOG_LEVEL_FATAL, "Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", expr, msg, file, line);
}
