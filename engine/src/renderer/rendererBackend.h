#pragma once

#include "rendererTypes.inl"

struct CODI_PlatformState;

b8 rendererBackendCreate(CODI_RendererBackendType type, struct CODI_PlatformState* platState, CODI_RendererBackend* outRendererBackend);

void rendererBackendDestroy(CODI_RendererBackend* rendererBackend);
