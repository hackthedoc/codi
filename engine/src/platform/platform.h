#pragma once

#include "macros.h"

typedef struct {
    void* internalState;
} platformState;

b8 platformSartup(platformState* platState, const char* appName, const i32 x, const i32 y, const i32 width, const i32 height);

void platformShutdown(platformState* platState);

b8 platformPumpMessage(platformState* platState);

CAPI void* platformAllocate(const u64 size, const b8 aligned);
CAPI void platformFree(void* block, const b8 aligned);
void* platformZeroMemory(void* block, const u64 size);
void* plateformCopyMemory(void* dest, const void* src, const u64 size);
void* platformSetMemory(void* dest, const i32 value, const u64 size);

void platformConsoleWrite(const char* msg, const u8 color);
void platformConsoleWriteError(const char* msg, const u8 color);

f64 platformGetAbsoluteTime();

// Sleep on the thread for the provided ms. This blocks the main thread.
// Should only be used for giving time back to the OS for unused update power.
// Therefore it is not exported.
void plateformSleep(const u64 ms);
