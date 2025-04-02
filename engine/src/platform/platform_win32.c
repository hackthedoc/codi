#include "platform.h"

// Windows platform layer
#if CPLATFORM_WINDOWS

#include "core/logger.h"
#include "core/input.h"

#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>

typedef struct {
    HINSTANCE hInstance;
    HWND hwnd;
} internalState;

// Clock

static f64 clockFrequency;
static LARGE_INTEGER startTime;

LRESULT CALLBACK win32_ProcessMessage(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam);

b8 platformSartup(CODI_PlatformState* platState, const char* appName, const i32 x, const i32 y, const i32 width, const i32 height) {
    platState->internalState = malloc(sizeof(internalState));
    internalState* state = (internalState*)platState->internalState;

    state->hInstance = GetModuleHandleA(0);

    // setup and register window class

    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);
    WNDCLASSA wc;
    memset(&wc, 0, sizeof(wc));
    wc.style = CS_DBLCLKS; // get double-clicks
    wc.lpfnWndProc = win32_ProcessMessage;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = state->hInstance;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszClassName = "codi_window_class";

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window registration failed", "Error", MB_ICONEXCLAMATION | MB_OK);
        return FALSE;
    }

    // create window

    const u32 clientX = x;
    const u32 clientY = y;
    const u32 clientWidth = width;
    const u32 clientHeight = height;


    u32 windowStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
    u32 windowExStyle = WS_EX_APPWINDOW;

    windowStyle |= WS_MAXIMIZEBOX;
    windowStyle |= WS_MINIMIZEBOX;
    windowStyle |= WS_THICKFRAME;

    // get border size

    RECT borderRect = {0,0,0,0};
    AdjustWindowRectEx(&borderRect, windowStyle, 0, windowExStyle);

    // define window size

    const u32 windowX = clientX + borderRect.left;
    const u32 windowY = clientY + borderRect.top;
    const u32 windowWidth = clientWidth + borderRect.right - borderRect.left;
    const u32 windowHeight = clientHeight + borderRect.bottom - borderRect.top;

    // create window

    HWND handle = CreateWindowExA(
        windowExStyle,
        "codi_window_class",
        appName,
        windowStyle,
        windowX,
        windowY,
        windowWidth,
        windowHeight,
        0,
        0,
        state->hInstance,
        0
    );

    if (handle == 0) {
        MessageBoxA(NULL, "Window creation failed!", "Error", MB_ICONEXCLAMATION | MB_OK);

        CFATAL("Window creation failed!");
        return FALSE;
    } else {
        state->hwnd = handle;
    }

    // show the window

    const b32 shouldActive = 1; // NOTE: if the window should not accept input, this should be false
    const i32 showWindowCommandFlags = shouldActive ? SW_SHOW : SW_SHOWNOACTIVATE;
    ShowWindow(state->hwnd, showWindowCommandFlags);

    // clock setup

    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    clockFrequency = 1.0 / (f64)freq.QuadPart;
    QueryPerformanceCounter(&startTime);

    return TRUE;
}

void platformShutdown(CODI_PlatformState* platState) {
    internalState* state = (internalState*)platState->internalState;

    if (state->hwnd) {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
}

b8 platformPumpMessage(CODI_PlatformState* platState) {
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return TRUE;
}

void* platformAllocate(const u64 size, const b8 aligned) {
    return malloc(size);
}

void platformFree(void* block, const b8 aligned) {
    free(block);
}

void* platformZeroMemory(void* block, const u64 size) {
    return memset(block, 0, size);
}

void* platformCopyMemory(void* dest, const void* src, const u64 size) {
    return memcpy(dest, src, size);
}

void* platformSetMemory(void* dest, const i32 value, const u64 size) {
    return memset(dest, value, size);
}

void platformConsoleWrite(const char* msg, const u8 color) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(consoleHandle, levels[color]);
    
    OutputDebugStringA(msg);
    const u64 length = strlen(msg);
    const LPDWORD numberWritten = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)length, numberWritten, 0); 
}

void platformConsoleWriteError(const char* msg, const u8 color) {
    HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    // FATAL, ERROR, WARN, INFO, DEBUG, TRACE
    static u8 levels[6] = {64, 4, 6, 2, 1, 8};
    SetConsoleTextAttribute(consoleHandle, levels[color]);
    
    OutputDebugStringA(msg);
    const u64 length = strlen(msg);
    const LPDWORD numberWritten = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)length, numberWritten, 0); 
}

f64 platformGetAbsoluteTime() {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (f64)currentTime.QuadPart * clockFrequency;
}

void platformSleep(const u64 ms) {
    Sleep(ms);
}

LRESULT CALLBACK win32_ProcessMessage(HWND hwnd, u32 msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
    case WM_ERASEBKGND:
        // notify the OS that erasing will be handled by the application to prevent flicker
        return 1;
    case WM_CLOSE:
        // TODO: fire an event for the application to quit
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE: {
        // TODO: fire event for window resize
    } break;
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP: {
        const b8 pressed = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
        const CODI_Key key = (u16)wParam;

        // pass to the input subsystem for processing
        inputProcessKey(key, pressed);
    } break;
    case WM_MOUSEMOVE: {
        const i32 x = GET_X_LPARAM(lParam);
        const i32 y = GET_Y_LPARAM(lParam);

        // pass to the input subsystem for processing
        inputProcessMouseMove(x, y);
    } break;
    case WM_MOUSEWHEEL: {
        i32 zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
        if (zDelta != 0) zDelta = (zDelta < 0) ? -1 : 1;
        
        // pass to the input subsystem for processing
        inputProcessMouseWheel(zDelta);
    } break;
    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_MBUTTONUP:
    case WM_RBUTTONUP: {
        const b8 pressed = (msg == WM_LBUTTONDOWN || msg == WM_MBUTTONDOWN || msg == WM_RBUTTONDOWN);
        CODI_MouseButton button = MOUSE_BUTTON_COUNT;
        switch (msg) {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            button = MOUSE_BUTTON_LEFT;
            break;
        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            button = MOUSE_BUTTON_MIDDLE;
            break;
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            button = MOUSE_BUTTON_RIGHT;
            break;
        }
        
        // pass to the input subsystem for processing
        if (button != MOUSE_BUTTON_COUNT) {
            inputProcessMouseButton(button, pressed);
        }
    } break;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

#endif
