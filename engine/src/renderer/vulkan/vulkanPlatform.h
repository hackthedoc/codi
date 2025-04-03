#pragma once

#include "macros.h"

/// @brief Appends the names of required extentions for this platform to the namesDArray, which should be created and passed in.
/// @param namesDArray a string DArray already created
void platformGetRequiredExtensionNames(const char*** namesDArray);
