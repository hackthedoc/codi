#include "rendererBackend.h"

#include "vulkan/vulkanBackend.h"

b8 rendererBackendCreate(CODI_RendererBackendType type, struct CODI_PlatformState* platState, CODI_RendererBackend* outRendererBackend) {
    outRendererBackend->platState = platState;

    if (type == RENDERER_BACKEND_TYPE_VULKAN) {
        outRendererBackend->initialize = vulkanRendererBackendInitialize;
        outRendererBackend->shutdown = vulkanRendererBackendShutdown;
        outRendererBackend->resized = vulkanRendererBackendOnResized;
        outRendererBackend->beginFrame = vulkanRendererBackendBeginFrame;
        outRendererBackend->endFrame = vulkanRendererBackendEndFrame;

        return TRUE;
    }

    return FALSE;
}

void rendererBackendDestroy(CODI_RendererBackend* rendererBackend) {
    rendererBackend->initialize = 0;
    rendererBackend->shutdown = 0;
    rendererBackend->beginFrame = 0;
    rendererBackend->endFrame = 0;
    rendererBackend->resized = 0;
}
