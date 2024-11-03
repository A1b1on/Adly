#include "main_window.h"
#include "main_window_config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MW_PRVT ((main_window_prvt_t*)(((main_window_t*)(main_window))->prvt))

typedef struct {
    WNDCLASS  window_class;
    HWND      hwnd;
    HINSTANCE hinstance;
    HMENU     menu;

} main_window_prvt_t;

static HRESULT main_window_register_window_class(main_window_t* main_window)
{
    // Clear the memory of the structure before use
    ZeroMemory(&MW_PRVT->window_class, sizeof(WNDCLASS));

    // Define the window class
    MW_PRVT->window_class.lpfnWndProc   = main_window_proc;
    MW_PRVT->window_class.hInstance     = MW_PRVT->hinstance;
    MW_PRVT->window_class.lpszClassName = MAIN_WINDOW_DEFAULT_CLASS_NAME;
    MW_PRVT->window_class.hIcon         = LoadIcon(NULL, MAIN_WINDOW_DEFAULT_ICON);
    MW_PRVT->window_class.hCursor       = LoadCursor(NULL, MAIN_WINDOW_DEFAULT_CURSOR);
    MW_PRVT->window_class.hbrBackground = MAIN_WINDOW_DEFAULT_BACKGROUND;

    // Register the window class
    if (!RegisterClass(&MW_PRVT->window_class)) {
        return E_FAIL;
    }

    return S_OK;
}
static void show_error_message(LPCWSTR message, HRESULT error_code)
{
    wchar_t full_message[512];
    swprintf(full_message, sizeof(full_message) / sizeof(wchar_t), L"%s\nError Code: 0x%08X", message, error_code);
    MessageBox(NULL, full_message, L"Error", MB_ICONERROR | MB_OK);
}
// TODO: Add logs
static HRESULT main_window_create_impl(main_window_t* main_window)
{
    HRESULT ret = S_OK;

    ret = main_window_register_window_class(main_window);
    if (ret != S_OK) {
        goto exit;
    }

    // Create the window
    MW_PRVT->hwnd = CreateWindowEx(
        MAIN_WINDOW_DEFAULT_EX_STYLE,                           // dwExStyle
        MW_PRVT->window_class.lpszClassName,                   // Class Name
        MAIN_WINDOW_DEFAULT_TITLE,                             // Window title
        MAIN_WINDOW_DEFAULT_STYLE,                             // dwStyle
        MAIN_WINDOW_DEFAULT_X, MAIN_WINDOW_DEFAULT_Y,          // X, Y
        MAIN_WINDOW_DEFAULT_WIDTH, MAIN_WINDOW_DEFAULT_HEIGHT, // W, H
        NULL,                                                  // HWND parent
        NULL,                                         // Menu
        MW_PRVT->hinstance,                                    // hInstance
        NULL                                                   // lpParam
    );

    if (MW_PRVT->hwnd == NULL) {
        DWORD error_code = GetLastError();
        wchar_t error_msg[512];
        swprintf(error_msg, sizeof(error_msg) / sizeof(wchar_t),
            L"Failed to create the main window.\nWindows Error Code: %lu (0x%08X)",
            error_code, error_code);
        MessageBox(NULL, error_msg, L"Window Creation Error", MB_ICONERROR | MB_OK);

        ret = E_POINTER;
        goto exit;
    }

exit:
    return ret;
}

// TODO: Add saving hmenu
static BOOL main_window_set_menu_impl(main_window_t* main_window, menu_t* menu)
{
    return SetMenu(MW_PRVT->hwnd, menu->get_menu(menu));
}

static HWND main_window_get_hwnd(main_window_t* main_window)
{
    return MW_PRVT->hwnd;
}

main_window_t* create_main_window(HINSTANCE hInstance)
{
    // Create pointer
    main_window_t* main_window = (main_window_t*)malloc(sizeof(main_window_t));
    if (main_window == NULL) {
        // TODO: Add logs
        return NULL;
    }

    // Create private part
    main_window->prvt = malloc(sizeof(main_window_prvt_t));
    if (main_window->prvt == NULL) {
        // TODO: Add logs
        free(main_window);
        return NULL;
    }

    // Set function list
    main_window->create   = main_window_create_impl;
    main_window->set_menu = main_window_set_menu_impl;
    main_window->get_hwnd = main_window_get_hwnd;

    // Set properties
    MW_PRVT->hinstance = hInstance;
    MW_PRVT->hinstance = NULL;

    return main_window;
}

