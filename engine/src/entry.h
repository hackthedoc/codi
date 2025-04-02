#pragma once

#include "core/application.h"
#include "core/logger.h"
#include "core/cmemory.h"

#include "game_types.h"

// Externally defined function to create a game.
extern b8 createGame(CODI_Game* outGame);

// The MAIN entry point of the application;
int main(void) {

    initializeMemory();

    // request the game instance for the application

    CODI_Game gInstance;
    if (!createGame(&gInstance)) {
        CFATAL("Could not create game!");
        return -1;
    }

    // ensure the function pointers exits

    if (!gInstance.initialize || !gInstance.update || !gInstance.render || !gInstance.onResize) {
        CFATAL("The game's function pointers must be assigned!");
        return -2;
    }

    // initialize application

    if (!CODI_ApplicationCreate(&gInstance)) {
        CINFO("Application failed to create!");
        return 1;
    }

    // start application loop

    if (!CODI_ApplicationRun()) {
        CINFO("Application did not shutdown gracefully.");
        return 2;
    }

    shutdownMemory();

    return 0;
}
