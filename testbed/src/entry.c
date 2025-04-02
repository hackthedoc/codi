#include "game.h"

#include <entry.h>

#include <core/cmemory.h>

// Define the function to create a game.
b8 createGame(CODI_Game* outGame) {

    // application configuration

    outGame->appConfig.startPosX = 100;
    outGame->appConfig.startPosY = 100;
    outGame->appConfig.startWidth = 1280;
    outGame->appConfig.startHeight = 720;
    outGame->appConfig.name = "Codi Engine Testbed";

    outGame->initialize = gameInitialize;
    outGame->update = gameUpdate;
    outGame->render = gameRender;
    outGame->onResize = gameOnResize;

    // create the game state

    outGame->state = CODI_Allocate(sizeof(CODI_GameState), MEMORY_TAG_GAME);

    return TRUE;
}
