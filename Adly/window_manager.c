#include "window_manager.h"

#include <stdlib.h>

#define WM_PRVT ((window_manager_prvt_t*)wm_instance->prvt)

typedef struct {
    WNDCLASS  window_class;
    HWND      hwnd;
    HINSTANCE hinstance;
} main_window_handler_t;

typedef struct {
    main_window_handler_t main_window_handler;
} window_manager_prvt_t;

// Static variable for the singleton instance
static window_manager_t* wm_instance = NULL;

static HRESULT register_main_window_class()
{
    // Clear the memory of the structure before use
    ZeroMemory(&WM_PRVT->main_window_handler.window_class, sizeof(WNDCLASS));

    // Define the window class
    WM_PRVT->main_window_handler.window_class.lpfnWndProc   = main_window_proc;
    WM_PRVT->main_window_handler.window_class.hInstance     = WM_PRVT->main_window_handler.hinstance;
    WM_PRVT->main_window_handler.window_class.lpszClassName = MAIN_WINDOW_CLASS_NAME;
    WM_PRVT->main_window_handler.window_class.hIcon         = LoadIcon(NULL, MAIN_WINDOW_ICON);
    WM_PRVT->main_window_handler.window_class.hCursor       = LoadCursor(NULL, IDC_ARROW); // Add a cursor
    WM_PRVT->main_window_handler.window_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);  // Background color


    // Register the window class
    if (!RegisterClass(&WM_PRVT->main_window_handler.window_class)) {
        return E_FAIL;
    }

    return S_OK;
}

static HRESULT create_main_window_impl(HINSTANCE hInstance)
{
    HRESULT ret = S_OK;

    WM_PRVT->main_window_handler.hinstance = hInstance;

    ret = register_main_window_class();
    if (ret != S_OK) {
        return ret;
    }

    // Create the window
    WM_PRVT->main_window_handler.hwnd = CreateWindowEx(
        0,
        WM_PRVT->main_window_handler.window_class.lpszClassName,
        MAIN_WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, // X, Y
        CW_USEDEFAULT, CW_USEDEFAULT, // W, H
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!WM_PRVT->main_window_handler.hwnd) {
        return E_FAIL;
    }

    ShowWindow(WM_PRVT->main_window_handler.hwnd, SW_SHOW);
    return S_OK;
}

// Function to access the singleton instance
window_manager_t* window_manager_instance(void)
{
    if (wm_instance == NULL) {
        wm_instance = (window_manager_t*)malloc(sizeof(window_manager_t));
        if (wm_instance == NULL) {
            exit(1);
        }

        wm_instance->prvt = malloc(sizeof(window_manager_prvt_t));
        if (wm_instance->prvt == NULL) {
            free(wm_instance);
            exit(1);
        }

        // Set methods
        wm_instance->create_main_window = create_main_window_impl;
    }

    return wm_instance;
}
