#pragma once

#include "macros.h"

// Returns the length of the given string
CAPI u64 CODI_StringLength(const char* str);

CAPI char* CODI_StringDuplicate(const char* str);

// Case sensitive string comparison.
CAPI b8 CODI_StringsEqual(const char* strA, const char* strB);
