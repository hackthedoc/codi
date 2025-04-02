#include "application.h"
#include "game_types.h"

#include "core/logger.h"
#include "core/cmemory.h"
#include "core/event.h"
#include "core/input.h"

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

// event handlers

b8 applicationOnEvent(const u16 code, void* sender, void* listener, CODI_EventContext context);
b8 applicationOnKey(const u16 code, void* sender, void* listener, CODI_EventContext context);

b8 CODI_ApplicationCreate(CODI_Game* gInstance) {
    if (inizialized) {
        CERROR("CODI_ApplicationCreate called more than once.");
        return FALSE;
    }

    appState.gInstance = gInstance;

    // Initialize subsystems

    initializeLogging();
    initializeInput();

    if (!initializeEvent()) {
        CERROR("Event system failed initialization. Application cannot continue.");
        return FALSE;
    }

    CODI_EventRegister(EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    CODI_EventRegister(EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    CODI_EventRegister(EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);

    // it's running

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

            // NOTE: Input update/state copying should always be handled after any input should be recorde; I.E. before this line.
            // As a safety, input is the last thing to be updated before the end of a frame.
            inputUpdate(0);
        }
    }

    appState.isRunning = FALSE;

    
    CODI_EventUnregister(EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    CODI_EventUnregister(EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    CODI_EventUnregister(EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);

    shutdownEvent();
    shutdownInput();

    platformShutdown(&appState.platform);

    return TRUE;
}


b8 applicationOnEvent(const u16 code, void* sender, void* listener, CODI_EventContext context) {
    switch (code) {
    case EVENT_CODE_APPLICATION_QUIT: {
        CINFO("EVENT_CODE_APPLICATION_QUIT recieved, shutting down.\n");
        appState.isRunning = FALSE;
        return TRUE;
    }
    }

    return FALSE;
}

b8 applicationOnKey(const u16 code, void* sender, void* listener, CODI_EventContext context) {
    if (code == EVENT_CODE_KEY_PRESSED) {
        const u16 keyCode = context.data.u16[0];
        if (keyCode == CODI_KEY_ESCAPE) {
            CODI_EventContext data = {};
            CODI_EventFire(EVENT_CODE_APPLICATION_QUIT, 0, data);

            return TRUE;
        }
    }

    return FALSE;
}
