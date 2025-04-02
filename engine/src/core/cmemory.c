#include "cmemory.h"

#include "core/logger.h"
#include "platform/platform.h"

// TODO: custom string lib
#include <string.h>
#include <stdio.h>

typedef struct {
    u64 totalAllocated;
    u64 taggedAllocations[MEMORY_TAG_COUNT];
} memoryStats;

static const char* memoryTagStrings[MEMORY_TAG_COUNT] = {
    "UNKNOWN    ",
    "ARRAY      ",
    "DARRAY     ",
    "DICT       ",
    "RING_QUEUE ",
    "BST        ",
    "STRING     ",
    "APPLICATION",
    "JOB        ",
    "TEXTURE    ",
    "MAT_INST   ",
    "RENDERER   ",
    "GAME       ",
    "TRANSFORM  ",
    "ENTITY     ",
    "ENTITY_NODE",
    "SCENE      ",
};

static memoryStats stats;

void initializeMemory() {
    platformZeroMemory(&stats, sizeof(stats));
}

void shutdownMemory() {

}

void* CODI_Allocate(const u64 size, const CODI_MemoryTag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        CWARN("CODI_Allocate called using MEMORY_TAG_UNKNOWN. Re-class this alloation.");
    }

    stats.totalAllocated += size;
    stats.taggedAllocations[tag] += size;

    // TODO: memory alignement
    void* block = platformAllocate(size, FALSE);
    platformZeroMemory(block, size);
    return block;
}

void  CODI_Free(void* block, const u64 size, const CODI_MemoryTag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        CWARN("CODI_Free called using MEMORY_TAG_UNKNOWN. Re-class this alloation.");
    }

    stats.totalAllocated -= size;
    stats.taggedAllocations[tag] -= size;

    // TODO: memory alignement
    platformFree(block, FALSE);
}

void* CODI_MemoryZero(void* block, const u64 size) {
    return platformZeroMemory(block, size);
}

void* CODI_MemoryCopy(void* dest, const void* src, const u64 size) {
    return platformCopyMemory(dest, src, size);
}

void* CODI_MemorySet(void* dest, const i32 value, const u64 size) {
    return platformSetMemory(dest, value, size);
}

char* CODI_GetMemoryUsageString() {
    const u64 kib = 1024;
    const u64 mib = 1024 * kib;
    const u64 gib = 1024 * mib;

    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);

    for (u32 i = 0; i < MEMORY_TAG_COUNT; i++) {
        char unit[4] = "XiB";
        float amount = 1.0f;
        
        if (stats.taggedAllocations[i] >= gib) {
            unit[0] = 'G';
            amount = stats.taggedAllocations[i] / (float)gib;
        } else if (stats.taggedAllocations[i] >= mib) {
            unit[0] = 'M';
            amount = stats.taggedAllocations[i] / (float)mib;
        } else if (stats.taggedAllocations[i] >= kib) {
            unit[0] = 'K';
            amount = stats.taggedAllocations[i] / (float)kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float)stats.taggedAllocations[i];
        }

        const i32 length = snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memoryTagStrings[i], amount, unit);
        offset += length;
    }

    char* ostr = _strdup(buffer);
    return ostr;
}
