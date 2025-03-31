#pragma once

// UNSIGNED INT TYPES

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

// SIGNED INT TYPES

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

// FLOATING POINT TYPES

typedef float f32;
typedef double f64;

// BOOLEAN TYPES

typedef char b8;
typedef int b32;

// STATIC ASSERTIONS

#if defined(__clang__) || defined(__gcc__)
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT static_assert
#endif

// TYPES SIZE VERIFICATION

STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

// PLATFORM DETECTION

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#define CPLATFORM_WINDOWS 1                                 // WINDOWS OS
#ifndef _WIN64
#error "64-bit is required on Windows!"
#endif
#elif defined(__linux__) || defined(__gnu_linux__)
#define CPLATFORM_LINUX 1                                   // LINUX OS
#if defined(__ANDROID__)
#define CPLATFORM_ANDROID 1                                 // ANDROID OS
#endif
#elif defined(__unix__)
#define CPLATFORM_UNIX 1                                    // UNIX OS
#elif defined(_POSIX_VERSION)
#define CPLATFORM_POSIX 1                                   // POSIX OS
#elif __APPLE__
#define CPLATFORM_APPLE 1                                   // APPLE OS
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
#define CPLATFORM_IOS 1                                     // IOS OS
#define CPLATFORM_IOS_SIMULATOR 1
#elif TARGET_OS_IPHONE
#define CPLATFORM_IOS 1                                     // IOS OS
#elif TARGET_OS_MAC
#define CPLATFORM_MAC                                       // MAC OS
#else
#error "Unknown Apple Platform."
#endif
#else
#error "Unknown platform!"
#endif

#ifdef CEXPORT
// EXPORTS
#ifdef _MSC_VER
#define CAPI __declspec(dllexport)
#else
#define CAPI __attribute__((visibility("default")))
#endif
#else
// IMPORTS
#ifdef _MSC_VER
#define CAPI __declspec(dllimport)
#else
#define CAPI
#endif
#endif
