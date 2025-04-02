#include "application.h"
#include "game_types.h"

#include "core/logger.h"
#include "core/cmemory.h"

#include "platform/platform.h"

typedef struct {
    CODI_Game* gInstance;
    b8 isRunning;
    b8 isSuspended;
    CODI_PlatformState platform;
    i16 width;
    i16 height;
    f64 lastTime;
} applicationState;

static b8 inizialized = FALSE;
static applicationState appState;

b8 CODI_ApplicationCreate(CODI_Game* gInstance) {
    if (inizialized) {
        CERROR("CODI_ApplicationCreate called more than once.");
        return FALSE;
    }

    appState.gInstance = gInstance;

    // Initialize subsystems

    initializeLogging();

    appState.isRunning = TRUE;
    appState.isSuspended = FALSE;

    // create platform state

    if (!platformSartup(
        &appState.platform,
        gInstance->appConfig.name,
        gInstance->appConfig.startPosX,
        gInstance->appConfig.startPosY,
        gInstance->appConfig.startWidth,
        gInstance->appConfig.startHeight
    )) {
        return FALSE;
    }

    // initialize the game

    if (!gInstance->initialize(gInstance)) {
        CFATAL("Game failed to initialized.");
        return FALSE;
    }

    gInstance->onResize(gInstance, appState.width, appState.height);

    inizialized = TRUE;

    return TRUE;
}

b8 CODI_ApplicationRun() {
    CINFO(CODI_GetMemoryUsageString());

    while (appState.isRunning) {
        if (!platformPumpMessage(&appState.platform)) {
            appState.isRunning = FALSE;
        }

        if (!appState.isSuspended) {
            if (!appState.gInstance->update(appState.gInstance, 0)) {
                CFATAL("Game update failed, shutting down.");
                appState.isRunning = FALSE;
                break;
            }
            
            if (!appState.gInstance->render(appState.gInstance, 0)) {
                CFATAL("Game render failed, shutting down.");
                appState.isRunning = FALSE;
                break;
            }
        }
    }

    appState.isRunning = FALSE;

    platformShutdown(&appState.platform);

    return TRUE;
}
