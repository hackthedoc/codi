#include "cstring.h"

#include "core/cmemory.h"

#include <string.h>

// TODO: make our own
u64 CODI_StringLength(const char* str) {
    return strlen(str);
}

char* CODI_StringDuplicate(const char* str) {
    const u64 length = CODI_StringLength(str) + 1;
    char* copy = CODI_Allocate(length, MEMORY_TAG_STRING);
    CODI_MemoryCopy(copy, str, length);
    return copy;
}

// TODO: make our own
b8 CODI_StringsEqual(const char* strA, const char* strB) {
    return strcmp(strA, strB) == 0;
}
