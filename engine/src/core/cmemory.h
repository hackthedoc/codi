#pragma once

#include "macros.h"

typedef enum {
    MEMORY_TAG_UNKNOWN,
    MEMORY_TAG_ARRAY,
    MEMORY_TAG_DARRAY,
    MEMORY_TAG_DICT,
    MEMORY_TAG_RING_QUEUE,
    MEMORY_TAG_BST,
    MEMORY_TAG_STRING,
    MEMORY_TAG_APPLICATION,
    MEMORY_TAG_JOB,
    MEMORY_TAG_TEXTURE,
    MEMORY_TAG_MATERIAL_INSTANCE,
    MEMORY_TAG_RENDERER,
    MEMORY_TAG_GAME,
    MEMORY_TAG_TRANSFORM,
    MEMORY_TAG_ENTITY,
    MEMORY_TAG_ENTITY_NODE,
    MEMORY_TAG_SCENE,

    MEMORY_TAG_COUNT
} CODI_MemoryTag;

CAPI void initializeMemory();
CAPI void shutdownMemory();

CAPI void* CODI_Allocate(const u64 size, const CODI_MemoryTag tag);
CAPI void  CODI_Free(void* block, const u64 size, const CODI_MemoryTag tag);
CAPI void* CODI_MemoryZero(void* block, const u64 size); // memory zero
CAPI void* CODI_MemoryCopy(void* dest, const void* src, const u64 size);
CAPI void* CODI_MemorySet(void* dest, const i32 value, const u64 size);

CAPI char* CODI_GetMemoryUsageString();
