#include "input.h"

#include "core/event.h"
#include "core/cmemory.h"
#include "core/logger.h"

typedef struct {
    b8 keys[256];
} keyboardState;

typedef struct {
    i16 x;
    i16 y;
    u8 buttons[MOUSE_BUTTON_COUNT];
} mouseState;

typedef struct {
    keyboardState kCurrent;
    keyboardState kPrevious;
    mouseState mCurrent;
    mouseState mPrevious; 
} inputState;

// internal input state
static inputState state = {};
static b8 initialized = FALSE;

void initializeInput() {
    CODI_MemoryZero(&state, sizeof(inputState));
    initialized = TRUE;
    CINFO("Input subsystem initialized.");
}

void shutdownInput() {
    initialized = FALSE;
}

void inputUpdate(const f64 deltaTime) {
    if (initialized == FALSE) return;

    CODI_MemoryCopy(&state.kPrevious, &state.kCurrent, sizeof(keyboardState));
    CODI_MemoryCopy(&state.mPrevious, &state.mCurrent, sizeof(mouseState));
}

void inputProcessKey(const CODI_Key key, const b8 pressed) {
    if (state.kCurrent.keys[key] == pressed) return;

    state.kCurrent.keys[key] = pressed;

    CODI_EventContext context;
    context.data.u16[0] = key;
    CODI_EventFire(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, context);
}

b8 CODI_IsKeyDown(const CODI_Key key) {
    if (initialized == FALSE) return FALSE;
    else return state.kCurrent.keys[key] == TRUE;
}

b8 CODI_IsKeyUp(const CODI_Key key) {
    if (initialized == FALSE) return TRUE;
    else return state.kCurrent.keys[key] == FALSE;
}

b8 CODI_WasKeyDown(const CODI_Key key) {
    if (initialized == FALSE) return FALSE;
    else return state.kPrevious.keys[key] == TRUE;
}

b8 CODI_WasKeyUp(const CODI_Key key) {
    if (initialized == FALSE) return TRUE;
    else return state.kPrevious.keys[key] == FALSE;
}

void inputProcessMouseButton(const CODI_MouseButton button, const b8 pressed) {
    if (state.mCurrent.buttons[button] == pressed) return;

    state.mCurrent.buttons[button] = pressed;

    CODI_EventContext context;
    context.data.u16[0] = button;
    CODI_EventFire(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_MOUSE_BUTTON_RELEASED, 0, context);
}

void inputProcessMouseMove(const i16 x, const i16 y) {
    if (state.mCurrent.x == x && state.mCurrent.y == y) return;

    // NOTE: enable this if debugging only
    //CDEBUG("Mouse pos: %i, %i", x, y);

    state.mCurrent.x = x;
    state.mCurrent.y = y;
    
    CODI_EventContext context;
    context.data.u16[0] = x;
    context.data.u16[1] = y;
    CODI_EventFire(EVENT_CODE_MOUSE_MOVED, 0, context);
}

void inputProcessMouseWheel(const i8 zDelta) {
    CODI_EventContext context;
    context.data.u8[0] = zDelta;
    CODI_EventFire(EVENT_CODE_MOUSE_WHEEL, 0, context);
}


CAPI b8 CODI_IsMouseButtonDown(const CODI_MouseButton button) {
    if (initialized == FALSE) return FALSE;
    else return state.mCurrent.buttons[button] == TRUE;
}

CAPI b8 CODI_IsMouseButtonUp(const CODI_MouseButton button) {
    if (initialized == FALSE) return TRUE;
    else return state.mCurrent.buttons[button] == FALSE;
}

CAPI b8 CODI_WasMouseButtonDown(const CODI_MouseButton button) {
    if (initialized == FALSE) return FALSE;
    else return state.mPrevious.buttons[button] == TRUE;
}

CAPI b8 CODI_WasMouseButtonUp(const CODI_MouseButton button) {
    if (initialized == FALSE) return TRUE;
    else return state.mPrevious.buttons[button] == FALSE;
}

CAPI void CODI_GetMousePosition(i32* x, i32* y) {
    if (initialized == FALSE) {
        *x = 0;
        *y = 0;
    }

    *x = state.mCurrent.x;
    *y = state.mCurrent.y;
}

CAPI void CODI_GetMousePreviousPosition(i32* x, i32* y) {
    if (initialized == FALSE) {
        *x = 0;
        *y = 0;
    }

    *x = state.mPrevious.x;
    *y = state.mPrevious.y;
}
