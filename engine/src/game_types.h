#pragma once

#include "core/application.h"

// Represents the basic game state in a game.
// Called for creation by the application.
typedef struct game {
    // The application configuration.
    applicationConfig appConfig;

    // Function pointer to game's initialize function.
    b8 (*initialize)(struct game* gInstance);

    // Function pointer to game's update function.
    b8 (*update)(struct game* gInstance, const f32 deltatTime);

    // Function pointer to game's render function.
    b8 (*render)(struct game* gInstance, const f32 deltatTime);

    // Function pointer to hyandle resizes, if applicable.
    void (*onResize)(struct game* gInstance, const u32 width, const u32 height);

    // Game-specific game state.
    // Created and managed by the game.
    void* state;
} game;
