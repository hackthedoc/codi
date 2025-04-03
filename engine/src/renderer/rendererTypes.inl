#pragma once

#include "macros.h"

typedef enum CODI_RendererBackendType {
    RENDERER_BACKEND_TYPE_VULKAN,
    RENDERER_BACKEND_TYPE_OPENGL,
    RENDERER_BACKEND_TYPE_DIRECTX
} CODI_RendererBackendType;

typedef struct CODI_RendererBackend {
    struct CODI_PlatformState* platState;
    u64 frameNumber;

    b8 (*initialize)(struct CODI_RendererBackend* backend, const char* appName, struct CODI_PlatformState* platState);

    void (*shutdown)(struct CODI_RendererBackend* backend);

    void (*resized)(struct CODI_RendererBackend* backend, const u16 width, const u16 height);

    b8 (*beginFrame)(struct CODI_RendererBackend* backend, const f32 deltaTime);
    b8 (*endFrame)(struct CODI_RendererBackend* backend, const f32 deltaTime);

} CODI_RendererBackend;

typedef struct CODI_RenderPacket {
    f32 deltaTime;
} CODI_RenderPacket;
