#pragma once

#include "core/application.h"
#include "core/logger.h"

#include "game_types.h"

// Externally defined function to create a game.
extern b8 createGame(game* outGame);

// The MAIN entry point of the application;
int main(void) {

    // request the game instance for the application

    game gInstance;
    if (!createGame(&gInstance)) {
        CFATAL("Could not create game!");
        return -1;
    }

    // ensure the function pointers exits

    if (!gInstance.initialize || !gInstance.update || !gInstance.render || gInstance.onResize) {
        CFATAL("The gamme's function pointers must be assigned!");
        return -2;
    }

    // initialize application

    if (!ApplicationCreate(&gInstance)) {
        CINFO("Application failed to create!");
        return 1;
    }

    // start application loop

    if (!ApplicationRun()) {
        CINFO("Application did not shutdown gracefully.");
        return 2;
    }

    return 0;
}
