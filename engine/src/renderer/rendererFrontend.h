#pragma once

#include "rendererTypes.inl"

struct CODI_StaticMeshData;
struct CODI_PlatformState;

b8 initializeRenderer(const char* appName, struct CODI_PlatformState* platState);
void shutdownRenderer();

void rendererOnResized(const u16 width, const u16 height);

b8 rendererDrawFrame(const CODI_RenderPacket* packet);
