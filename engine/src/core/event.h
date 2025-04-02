#pragma once

#include "macros.h"

typedef struct {
    // 128 bits
    union {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];
        
        i32 i32[4];
        u32 u32[4];
        f32 f32[4];
        
        i16 i16[8];
        u16 u16[8];
        
        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;
} CODI_EventContext;

// return true if handled
typedef b8 (*PFN_OnEvent)(const u16 code, void* sender, void* listener, CODI_EventContext data);

b8 initializeEvent();
void shutdownEvent();

/// @brief Register to listen for when events are sent with the provided code. Event with duplicate listener/callback combos will not be registered again and will cause this to return FALSE.
/// @param code The event code to listen for.
/// @param listener A pointer to a listener instance. Can be NULL.
/// @param onEvent The callback function pointer to be invoked when the event code is fired.
/// @return TRUE if the event is successfully registered; else FALSE.
CAPI b8 CODI_EventRegister(const u16 code, void* listener, PFN_OnEvent onEvent);

/// @brief Unregister from listening for when events are sent with the provided code. If no matching registration is found, this function returns FALSE.
/// @param code The event code to stop listening for.
/// @param listener A pointer to a listener instance. Can be NULL.
/// @param onEvent The callback function pointer to be unregistered.
/// @return TRUE if the event is successfully unregistered; else FALSE.
CAPI b8 CODI_EventUnregister(const u16 code, void* listener, PFN_OnEvent onEvent);

/// @brief Fires an event  to listeners of the given code. If an event handler returns TRUE, the event is considered handled and is not passed on to any more listeners.
/// @param code The event code to fire.
/// @param sender A pointer to the sender. Can be NULL.
/// @param data The event data.
/// @return TRUE if handled; else FALSE.
CAPI b8 CODI_EventFire(const u16 code, void* sender, const CODI_EventContext data);

// System internal event code. Application should use codes beyond 255.
typedef enum {
    // Shuts the application down on the next frame.
    EVENT_CODE_APPLICATION_QUIT = 0x01,

    // Keyboard key pressed.
    // usage: 
    // u16 keyCode = data.data.u16[0];
    EVENT_CODE_KEY_PRESSED = 0x02,

    // Keyboard key released.
    // usage: 
    // u16 keyCode = data.data.u16[0];
    EVENT_CODE_KEY_RELEASED = 0x03,

    // Mouse button pressed.
    // usage: 
    // u16 button = data.data.u16[0];
    EVENT_CODE_MOUSE_BUTTON_PRESSED = 0x04,

    // Mouse button released.
    // usage:
    // u16 button = data.data.u16[0];
    EVENT_CODE_MOUSE_BUTTON_RELEASED = 0x05,

    // Mouse moved.
    // usage:
    // u16 x = data.data.u16[0];
    // u16 y = data.data.u16[1];
    EVENT_CODE_MOUSE_MOVED = 0x06,

    // Mouse wheeled.
    // usage: 
    // u8 zDelta = data.data.u8[0];
    EVENT_CODE_MOUSE_WHEEL = 0x07,

    // Resized/resolution changed from the OS.
    // usage: 
    // u16 width = data.data.u16[0];
    // u16 height = data.data.u16[1];
    EVENT_CODE_RESIZED = 0x08,

    MAX_EVENT_CODE = 0xFF
} CODI_EventCode;
