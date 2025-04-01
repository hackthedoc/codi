#pragma once

#include <macros.h>
#include <game_types.h>

typedef struct {
    f32 deltaTime;
} gameState;

b8 gameInitialize(game* gInstance);

b8 gameUpdate(game* gInstance, const f32 deltaTime);

b8 gameRender(game* gInstance, const f32 deltaTime);

void gameOnResize(game* gInstance, const u32 width, const u32 height);
