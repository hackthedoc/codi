#include "darray.h"

#include "core/logger.h"
#include "core/cmemory.h"

void* _darrayCreate(const u64 capacity, const u64 stride) {
    const u64 headerSize = DARRAY_FIELD_LENGTH * sizeof(u64);
    const u64 arraySize = capacity * stride;
    const u64 size = headerSize+arraySize;
    u64* newArray = CODI_Allocate(size, MEMORY_TAG_DARRAY);
    CODI_MemorySet(newArray, 0, size);
    newArray[DARRAY_CAPACITY] = capacity;
    newArray[DARRAY_LENGTH] = 0;
    newArray[DARRAY_STRIDE] = stride;
    return (void*)(newArray+DARRAY_FIELD_LENGTH);
}

void _darrayDestroy(void* array) {
    u64* header = (u64*)array - DARRAY_FIELD_LENGTH;
    const u64 headerSize = DARRAY_FIELD_LENGTH * sizeof(u64);
    const u64 totalSize = headerSize + header[DARRAY_CAPACITY] + header[DARRAY_STRIDE];
    CODI_Free(header, totalSize, MEMORY_TAG_DARRAY);
}

u64 _darrayFieldGet(void* array, const u64 field) {
    u64* header = (u64*)array - DARRAY_FIELD_LENGTH;
    return header[field];
}

void _darrayFieldSet(void* array, const u64 field, const u64 value) {
    u64* header = (u64*)array - DARRAY_FIELD_LENGTH;
    header[field] = value;
}

void* _darrayResize(void* array) {
    const u64 length = CODI_DArrayLength(array);
    const u64 stride = CODI_DArrayStride(array);
    void* temp = _darrayCreate(DARRAY_RESIZE_FACTOR*CODI_DArrayCapacity(array), stride);
    CODI_MemoryCopy(temp, array, length*stride);
    CODI_DArraySetLength(temp, length);
    CODI_DArrayDestroy(array);
    return temp;
}

void* _darrayPush(void* array, const void* vptr) {
    const u64 length = CODI_DArrayLength(array);
    const u64 stride = CODI_DArrayStride(array);
    if (length >= CODI_DArrayCapacity(array)) {
        array = _darrayResize(array);
    }

    u64 addr = (u64)array;
    addr += length*stride;
    CODI_MemoryCopy((void*)addr, vptr, stride);
    CODI_DArraySetLength(array, length+1);
    return array;
}

void _darrayPop(void* array, void* dest) {
    const u64 length = CODI_DArrayLength(array) - 1;
    const u64 stride = CODI_DArrayStride(array);
    u64 addr = (u64)array;
    addr += length*stride;
    CODI_MemoryCopy(dest, (void*)addr, stride);
    CODI_DArraySetLength(array, length);
}

void* _darrayInsertAt(void* array, const u64 index, const void* vptr) {
    const u64 length = CODI_DArrayLength(array);
    const u64 stride = CODI_DArrayStride(array);
    if (index >= length) {
        CERROR("Index out of bounds! length: %i, index: %i", length, index);
        return array;
    }
    if (length >= CODI_DArrayCapacity(array)) {
        array = _darrayResize(array);
    }

    u64 addr = (u64)array;

    // if not on the last element, move the rest outward
    if (index != length-1) {
        CODI_MemoryCopy(
            (void*)(addr + ((index+1)   * stride)),
            (void*)(addr + (index       * stride)),
            (length - index) * stride
        );
    }

    CODI_MemoryCopy((void*)(addr + (index * stride)), vptr, stride);

    CODI_DArraySetLength(array, length+1);
    return array;
}

void* _darrayPopAt(void* array, const u64 index, void* dest) {
    const u64 length = CODI_DArrayLength(array);
    const u64 stride = CODI_DArrayStride(array);
    if (index >= length) {
        CERROR("Index out of bounds! length: %i, index: %i", length, index);
        return array;
    }

    u64 addr = (u64)array;
    CODI_MemoryCopy(dest, (void*)(addr + (index * stride)), stride);

    // if not on the last element, move the rest inward
    if (index != length-1) {
        CODI_MemoryCopy(
            (void*)(addr + (index       * stride)),
            (void*)(addr + ((index+1)   * stride)),
            (length - index) * stride
        );
    }

    CODI_DArraySetLength(array, length-1);
    return array;
}
