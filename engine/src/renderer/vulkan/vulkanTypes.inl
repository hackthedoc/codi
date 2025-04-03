#pragma once

#include "macros.h"

#include "core/asserts.h"

#include <vulkan/vulkan.h>

// Checks the given expression's return value against VK_SUCCESS
#define VK_CHECK(expr) CASSERT(expr == VK_SUCCESS)

typedef struct CODI_VulkanContext {
    VkInstance instance;
    VkAllocationCallbacks* allocator;

#if defined(_DEBUG)
    VkDebugUtilsMessengerEXT debugMessenger;
#endif
} CODI_VulkanContext;
