#pragma once

#include "macros.h"

#include <vulkan/vulkan.h>

typedef struct CODI_VulkanContext {
    VkInstance instance;
    VkAllocationCallbacks* allocator;
} CODI_VulkanContext;
