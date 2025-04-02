#pragma once

#include "macros.h"

/*
Memory Layout
u64 capacity = number of elements that can be held
u64 length = number of elements currently contained
u64 stride = size of each element in bytes
void* elements
*/

enum {
    DARRAY_CAPACITY,
    DARRAY_LENGTH,
    DARRAY_STRIDE,
    DARRAY_FIELD_LENGTH
};

CAPI void* _darrayCreate(const u64 capacity, const u64 stride);
CAPI void _darrayDestroy(void* array);

CAPI u64 _darrayFieldGet(void* array, const u64 field);
CAPI void _darrayFieldSet(void* array, const u64 field, const u64 value);

void* _darrayResize(void* array);

CAPI void* _darrayPush(void* array, const void* vptr);
CAPI void _darrayPop(void* array, void* dest);

CAPI void* _darrayInsertAt(void* array, const u64 index, const void* vptr);
CAPI void* _darrayPopAt(void* array, const u64 index, void* dest);

#define DARRAY_DEFAULT_CAPACITY 1
#define DARRAY_RESIZE_FACTOR 2

#define CODI_DArrayCreate(type) _darray_create(DARRAY_DEFAULT_CAPACITY, sizeof(type))

#define CODI_DArrayReserve(type, capacity) _darray_create(capacity, sizeof(type))

#define CODI_DArrayDestroy(array) _darray_destroy(array)

#define CODI_DArrayPush(array, value)       \
    {                                       \
       typeof(value) t = value;             \
       array = _darrayPush(array, &temp);   \ 
    }

#define CODI_DArrayPop(array, valuePtr) _darrayPop(array, valuePtr)

#define CODI_DArrayInsertAt(array, index, value)    \
    {                                               \
       typeof(value) t = value;                     \
       array = _darrayPush(array, index, &temp);    \ 
    }

#define CODI_DArrayPopAt(array, index, valuePtr) _darrayPopAt(array, index, valuePtr)

#define CODI_DArrayClear(array) _darrayFieldSet(array, DARRAY_LENGTH, 0)

#define CODI_DArrayCapacity(array) _darrayFieldGet(array, DARRAY_CAPACITY)

#define CODI_DArrayLength(array) _darrayFieldGet(array, DARRAY_LENGTH)

#define CODI_DArrayStride(array) _darrayFieldGet(array, DARRAY_STRIDE)

#define CODI_DArraySetLength(array, value) _darrayFieldSet(array, DARRAY_LENGTH, value)
