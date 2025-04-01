#pragma once

#include "macros.h"

typedef struct {
    void* internalState;
} platformState;

CAPI b8 platform_Sartup(platformState* platState, const char* appName, const i32 x, const i32 y, const i32 width, const i32 height);

CAPI void platform_Shutdown(platformState* platState);

CAPI b8 platform_PumpMessage(platformState* platState);

void* platform_Allocate(const u64 size, const b8 aligned);
void platform_Free(void* block, const b8 aligned);
void* platform_ZeroMemory(void* block, const u64 size);
void* plateform_CopyMemory(void* dest, const void* src, const u64 size);
void* platform_SetMemory(void* dest, const i32 value, const u64 size);

void platform_ConsoleWrite(const char* msg, const u8 color);
void platform_ConsoleWriteError(const char* msg, const u8 color);

f64 platform_GetAbsoluteTime();

// Sleep on the thread for the provided ms. This blocks the main thread.
// Should only be used for giving time back to the OS for unused update power.
// Therefore it is not exported.
void plateform_Sleep(const u64 ms);
