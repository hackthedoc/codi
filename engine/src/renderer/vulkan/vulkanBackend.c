#include "vulkanBackend.h"

#include "vulkanTypes.inl"
#include "vulkanPlatform.h"

#include "core/logger.h"

#include "containers/cstring.h"
#include "containers/darray.h"

#include "platform/platform.h"

// backend vulkan context
static CODI_VulkanContext context;

VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messaageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
);

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

    // required extentions

    const char** requiredExtensions = CODI_DArrayCreate(const char*);
    CODI_DArrayPush(requiredExtensions, &VK_KHR_SURFACE_EXTENSION_NAME);        // generic surface extension
    platformGetRequiredExtensionNames(&requiredExtensions);                     // platform specific extension(s)
#if defined(_DEBUG)
    CODI_DArrayPush(requiredExtensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);    // debug utils

    CDEBUG("Required extansions:");
    const u32 length = CODI_DArrayLength(requiredExtensions);
    for (u32 i = 0; i < length; i++) {
        CDEBUG(requiredExtensions[i]);
    }
#endif

    createInfo.enabledExtensionCount = CODI_DArrayLength(requiredExtensions);
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    // validation layers

    const char** requiredLayers = 0;
    u32 requiredLayerCount = 0;

// If validation should be done, get a list of the required validation layer names and make sure they exist.
// NOTE: Validation layers should only be enabled on non-released build
#if defined(_DEBUG)
    CINFO("Validation layers enabled. Enumerating...");

    // list of layers required

    requiredLayers = CODI_DArrayCreate(const char*);
    CODI_DArrayPush(requiredLayers, &"VK_LAYER_KHRONOS_validation");
    requiredLayerCount = CODI_DArrayLength(requiredLayers);

    // list of available layers

    u32 availableLayerCount = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, 0));
    VkLayerProperties* availableLayers = CODI_DArrayReserve(VkLayerProperties, availableLayerCount);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers));

    // verify that all required layers are available
    for (u32 i = 0; i < requiredLayerCount; i++) {
        CINFO("Searching for layer: %s...", requiredLayers[i]);
        b8 found = FALSE;
        u32 j = 0;
        while (j < availableLayerCount && !found) {
            if (CODI_StringsEqual(requiredLayers[i], availableLayers[j].layerName)) {
                found = TRUE;
                CINFO("Found.");
            }
            j++;
        }

        if (!found) {
            CFATAL("Required validation layer is missing: %s", requiredLayers[i]);
            return FALSE;
        }
    }
    CINFO("All required validation layers are present.");
#endif

    createInfo.enabledLayerCount = requiredLayerCount;
    createInfo.ppEnabledLayerNames = requiredLayers;

    // create the instance

    VK_CHECK(vkCreateInstance(&createInfo, context.allocator, &context.instance));
    CINFO("Vulkan instance created.");

    // Debuger
#if defined(_DEBUG)
    CDEBUG("Creating Vulkan debugger...");
    const u32 logSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT   |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;   //|
                            //VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
    
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT};
    debugCreateInfo.messageSeverity = logSeverity;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
    debugCreateInfo.pfnUserCallback = vkDebugCallback;
    
    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkCreateDebugUtilsMessengerEXT");
    CASSERT_MSG(func, "Failed to create debug messenger!");
    VK_CHECK(func(context.instance, &debugCreateInfo, context.allocator, &context.debugMessenger));

    CDEBUG("Vulkan debugger created.");
#endif

    CINFO("Vulkan renderer initialized successfully.");
    return TRUE;
}

void vulkanRendererBackendShutdown(struct CODI_RendererBackend* backend) {

#if defined(_DEBUG)
    CDEBUG("Destroying Vulkan debugger...");
    if (context.debugMessenger) {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(context.instance, "vkDestroyDebugUtilsMessengerEXT");
        func(context.instance, context.debugMessenger, context.allocator);
    }
    context.debugMessenger = 0;
#endif

    CDEBUG("Destroying Vulkan instance...");
    vkDestroyInstance(context.instance, context.allocator);
}

void vulkanRendererBackendOnResized(struct CODI_RendererBackend* backend, const u16 width, const u16 height) {

}

b8 vulkanRendererBackendBeginFrame(struct CODI_RendererBackend* backend, const f32 deltaTime) {
    return TRUE;
}

b8 vulkanRendererBackendEndFrame(struct CODI_RendererBackend* backend, const f32 deltaTime) {
    return TRUE;
}

VKAPI_ATTR VkBool32 VKAPI_CALL vkDebugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messaageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
    void* userData
) {
    switch (messageSeverity) {
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        CTRACE(callbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        CINFO(callbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        CWARN(callbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    default:
        CERROR(callbackData->pMessage);
        break;
    }

    return VK_FALSE;
}
