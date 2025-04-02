#pragma once

#include "core/application.h"

// Represents the basic game state in a game.
// Called for creation by the application.
typedef struct CODI_Game {
    // The application configuration.
    CODI_ApplicationConfig appConfig;

    // Function pointer to game's initialize function.
    b8 (*initialize)(struct CODI_Game* gInstance);

    // Function pointer to game's update function.
    b8 (*update)(struct CODI_Game* gInstance, const f32 deltatTime);

    // Function pointer to game's render function.
    b8 (*render)(struct CODI_Game* gInstance, const f32 deltatTime);

    // Function pointer to hyandle resizes, if applicable.
    void (*onResize)(struct CODI_Game* gInstance, const u32 width, const u32 height);

    // Game-specific game state.
    // Created and managed by the game.
    void* state;
} CODI_Game;
