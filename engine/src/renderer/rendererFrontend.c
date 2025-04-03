#include "rendererFrontend.h"

#include "rendererBackend.h"

#include "core/logger.h"
#include "core/cmemory.h"

// backend render context
static CODI_RendererBackend* backend = 0;

b8 initializeRenderer(const char* appName, struct CODI_PlatformState* platState) {
    backend = CODI_Allocate(sizeof(CODI_RendererBackend), MEMORY_TAG_RENDERER);

    // TODO: make this configurable
    rendererBackendCreate(RENDERER_BACKEND_TYPE_VULKAN, platState, backend);

    backend->frameNumber = 0;

    if (!backend->initialize(backend, appName, platState)) {
        CFATAL("Renderer backend failed to initialized. Shutting down.");
        return FALSE;
    }

    return TRUE;
}

void shutdownRenderer() {
    backend->shutdown(backend);
    CODI_Free(backend, sizeof(CODI_RendererBackend), MEMORY_TAG_RENDERER);
}

void rendererOnResized(const u16 width, const u16 height) {

}

b8 rendererBeginFrame(const CODI_RenderPacket* packet) {
    return backend->beginFrame(backend, packet->deltaTime);
}

b8 rendererEndFrame(const CODI_RenderPacket* packet) {
    return backend->endFrame(backend, packet->deltaTime);
    backend->frameNumber++;
}

b8 rendererDrawFrame(const CODI_RenderPacket* packet) {
    if (!rendererBeginFrame(packet)) return TRUE;

    if (!rendererEndFrame(packet)) {
        CERROR("rendererEndFrame failed. Shutting Down.");
        return FALSE;
    }

    return TRUE;
}
