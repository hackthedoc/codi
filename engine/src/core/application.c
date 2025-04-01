#include "application.h"
#include "game_types.h"

#include "logger.h"

#include "platform/platform.h"

typedef struct {
    game* gInstance;
    b8 isRunning;
    b8 isSuspended;
    platformState platform;
    i16 width;
    i16 height;
    f64 lastTime;
} applicationState;

static b8 inizialized = FALSE;
static applicationState appState;

b8 ApplicationCreate(game* gInstance) {
    if (inizialized) {
        CERROR("ApplicationCreate called more than once.");
        return FALSE;
    }

    appState.gInstance = gInstance;

    // Initialize subsystems

    initializeLogging();

    // TODO: remove this

    CFATAL("A test message: %f", 3.14f);
    CERROR("A test message: %f", 3.14f);
    CWARN("A test message: %f", 3.14f);
    CINFO("A test message: %f", 3.14f);
    CDEBUG("A test message: %f", 3.14f);
    CTRACE("A test message: %f", 3.14f);

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

b8 ApplicationRun() {
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
