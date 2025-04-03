#pragma once

#include "renderer/rendererBackend.h"

b8 vulkanRendererBackendInitialize(struct CODI_RendererBackend* backend, const char* appName, struct CODI_PlatformState* platState);

void vulkanRendererBackendShutdown(struct CODI_RendererBackend* backend);

void vulkanRendererBackendOnResized(struct CODI_RendererBackend* backend, const u16 width, const u16 height);

b8 vulkanRendererBackendBeginFrame(struct CODI_RendererBackend* backend, const f32 deltaTime);
b8 vulkanRendererBackendEndFrame(struct CODI_RendererBackend* backend, const f32 deltaTime);