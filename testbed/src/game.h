#pragma once

#include <macros.h>
#include <game_types.h>

typedef struct {
    f32 deltaTime;
} CODI_GameState;

b8 gameInitialize(CODI_Game* gInstance);

b8 gameUpdate(CODI_Game* gInstance, const f32 deltaTime);

b8 gameRender(CODI_Game* gInstance, const f32 deltaTime);

void gameOnResize(CODI_Game* gInstance, const u32 width, const u32 height);
