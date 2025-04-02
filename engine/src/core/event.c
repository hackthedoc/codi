#include "event.h"

#include "core/cmemory.h"

#include "containers/darray.h"

#define MAX_MESSAGE_CODES 16384

typedef struct {
    void* listener;
    PFN_OnEvent callback;
} registeredEvent;

typedef struct {
    registeredEvent* events;
} eventCodeEntry;

typedef struct {
    eventCodeEntry registered[MAX_MESSAGE_CODES];
} eventSystemState;

// Event system internal state.
static eventSystemState state;
static b8 initialized = FALSE;

b8 initializeEvent() {
    if (initialized == TRUE) return FALSE;

    initialized = FALSE;

    CODI_MemoryZero(&state, sizeof(state));

    initialized = TRUE;
    
    return TRUE;
}

void shutdownEvent() {
    for (u16 i = 0; i < MAX_MESSAGE_CODES; i++) {
        if(state.registered[i].events != 0) {
            CODI_DArrayDestroy(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

b8 CODI_EventRegister(const u16 code, void* listener, PFN_OnEvent onEvent) {
    if (initialized == FALSE) return FALSE;

    if (state.registered[code].events == 0) {
        state.registered[code].events = CODI_DArrayCreate(registeredEvent);
    }

    // check if the event is already registered

    const u64 registeredCount = CODI_DArrayLength(state.registered[code].events);
    for (u64 i = 0; i < registeredCount; i++) {
        if (state.registered[code].events[i].listener == listener) {
            // TODO: warn
            return FALSE;
        }
    }

    // proceed with registration

    registeredEvent event = {
        .listener = listener,
        .callback = onEvent
    };
    CODI_DArrayPush(state.registered[code].events, event);

    return TRUE;
}

b8 CODI_EventUnregister(const u16 code, void* listener, PFN_OnEvent onEvent) {
    if (initialized == FALSE) return FALSE;
    
    // if nothing registered, get out
    if (state.registered[code].events == 0) {
        // TODO: warn
        return FALSE;
    }
    
    const u64 registeredCount = CODI_DArrayLength(state.registered[code].events);
    for (u64 i = 0; i < registeredCount; i++) {
        const registeredEvent e = state.registered[code].events[i];
        if (e.listener == listener && e.callback == onEvent) {
            registeredEvent poppedEvent;
            CODI_DArrayPopAt(state.registered[code].events, i, &poppedEvent);
            return TRUE;
        }
    }

    // not found
    return FALSE;
}

b8 CODI_EventFire(const u16 code, void* sender, const CODI_EventContext data) {
    if (initialized == FALSE) return FALSE;

    // if nothing registered, get out
    if (state.registered[code].events == 0) {
        return FALSE;
    }

    const u64 registeredCount = CODI_DArrayLength(state.registered[code].events);
    for (u64 i = 0; i < registeredCount; i++) {
        const registeredEvent e = state.registered[code].events[i];
        if (e.callback(code, sender, e.listener, data)) {
            return TRUE; // event handled, do not send to other listeners
        }
    }

    // not found
    return FALSE;
}
