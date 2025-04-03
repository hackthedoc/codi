#include "vulkanBackend.h"

#include "vulkanTypes.inl"

#include "core/logger.h"

// backend vulkan context
static CODI_VulkanContext context;

b8 vulkanRendererBackendInitialize(struct CODI_RendererBackend* backend, const char* appName, struct CODI_PlatformState* platState) {

    // TODO: custom allocator
    context.allocator = 0;

    // setup vulkan instance

    VkApplicationInfo appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.apiVersion = VK_API_VERSION_1_4;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Codi Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    VkInstanceCreateInfo createInfo = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = 0;
    createInfo.ppEnabledExtensionNames = 0;
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = 0;

    const VkResult result = vkCreateInstance(&createInfo, context.allocator, &context.instance);
    if (result != VK_SUCCESS) {
        CERROR("vkCreateInstance failed with result: %u", result);
        return FALSE;
    }

    CINFO("Vulkan renderer initialized successfully.");
    return TRUE;
}

void vulkanRendererBackendShutdown(struct CODI_RendererBackend* backend) {

}

void vulkanRendererBackendOnResized(struct CODI_RendererBackend* backend, const u16 width, const u16 height) {

}

b8 vulkanRendererBackendBeginFrame(struct CODI_RendererBackend* backend, const f32 deltaTime) {
    return TRUE;
}

b8 vulkanRendererBackendEndFrame(struct CODI_RendererBackend* backend, const f32 deltaTime) {
    return TRUE;
}
