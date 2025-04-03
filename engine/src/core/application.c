#include "application.h"
#include "game_types.h"

#include "core/logger.h"
#include "core/cmemory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"

#include "platform/platform.h"

#include "renderer/rendererFrontend.h"

typedef struct {
    CODI_Game* gInstance;
    b8 isRunning;
    b8 isSuspended;
    CODI_PlatformState platform;
    i16 width;
    i16 height;
    CODI_Clock clock;
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

    // renderer startup

    if (!initializeRenderer(gInstance->appConfig.name, &appState.platform)) {
        CFATAL("Failed to initialize renderer. Aborting application.");
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
    clockStart(&appState.clock);
    clockUpdate(&appState.clock);
    appState.lastTime = appState.clock.elapsed;
    f64 runningTime = 0;
    u8 frameCount = 0;
    f64 targetFrameTime = 1.f / 60;

    CINFO(CODI_GetMemoryUsageString());

    while (appState.isRunning) {
        if (!platformPumpMessage(&appState.platform)) {
            appState.isRunning = FALSE;
        }

        if (!appState.isSuspended) {
            clockUpdate(&appState.clock);
            const f64 currentTime = appState.clock.elapsed;
            const f64 delta = currentTime - appState.lastTime;
            const f64 frameStartTime = platformGetAbsoluteTime();

            // update routine

            if (!appState.gInstance->update(appState.gInstance, delta)) {
                CFATAL("Game update failed, shutting down.");
                appState.isRunning = FALSE;
                break;
            }

            // render routine
            
            if (!appState.gInstance->render(appState.gInstance, delta)) {
                CFATAL("Game render failed, shutting down.");
                appState.isRunning = FALSE;
                break;
            }

            CODI_RenderPacket packet;
            packet.deltaTime = delta;
            rendererDrawFrame(&packet);

            // time check

            const f64 frameEndTime = platformGetAbsoluteTime();
            const f64 frameElapsedTime = frameEndTime - frameStartTime;
            runningTime += frameElapsedTime;
            const f64 remainingTime = targetFrameTime - frameElapsedTime;

            if (remainingTime > 0) {
                const u64 remainingMS = remainingTime * 1000;

                // NOTE: if there's time left, give it back to the OS.
                b8 limitFrames = FALSE;

                if (limitFrames & (remainingMS > 0)) {
                    platformSleep(remainingMS-1);
                }

                frameCount++;
            }

            // NOTE: Input update/state copying should always be handled after any input should be recorde; I.E. before this line.
            // As a safety, input is the last thing to be updated before the end of a frame.
            inputUpdate(delta);

            appState.lastTime = currentTime;
        }
    }

    appState.isRunning = FALSE;

    CODI_EventUnregister(EVENT_CODE_APPLICATION_QUIT, 0, applicationOnEvent);
    CODI_EventUnregister(EVENT_CODE_KEY_PRESSED, 0, applicationOnKey);
    CODI_EventUnregister(EVENT_CODE_KEY_RELEASED, 0, applicationOnKey);

    shutdownEvent();
    shutdownInput();
    shutdownRenderer();
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
