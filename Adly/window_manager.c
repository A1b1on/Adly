#include "window_manager.h"

#include <CommCtrl.h>
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

static void create_main_menu(HWND hwnd)
{
    HMENU hMenu = CreateMenu();

    HMENU hFileMenu = CreateMenu();
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_NEW,  L"New");
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_OPEN, L"Open");
    AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
    AppendMenu(hFileMenu, MF_STRING, ID_FILE_EXIT, L"Exit");

    HMENU hEditMenu = CreateMenu();
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_UNDO, L"Undo");
    AppendMenu(hEditMenu, MF_STRING, ID_EDIT_REDO, L"Redo");

    HMENU hBuildMenu = CreateMenu();
    AppendMenu(hBuildMenu, MF_STRING, ID_BUILD_COMPILE, L"Compile");
    AppendMenu(hBuildMenu, MF_STRING, ID_BUILD_RUN,     L"Run");

    HMENU hHelpMenu = CreateMenu();
    AppendMenu(hHelpMenu, MF_STRING, ID_HELP_ABOUT, L"About");

    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu,  L"File");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hEditMenu,  L"Edit");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hBuildMenu, L"Build");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu,  L"Help");

    SetMenu(hwnd, hMenu);
}

static HWND create_toolbar(HWND hwnd)
{
    HWND hToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0, hwnd, NULL, GetModuleHandle(NULL), NULL);

    TBADDBITMAP tbBitmap = { 0 };
    tbBitmap.hInst       = HINST_COMMCTRL;
    tbBitmap.nID         = IDB_STD_SMALL_COLOR;
    SendMessage(hToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbBitmap);

    TBBUTTON tbb[3]  = { 0 };
    tbb[0].iBitmap   = 0;
    tbb[0].idCommand = ID_TOOLBAR_NEW;
    tbb[0].fsState   = TBSTATE_ENABLED;
    tbb[0].fsStyle   = TBSTYLE_BUTTON;
    tbb[0].iString   = (INT_PTR)L"New";

    tbb[1].iBitmap   = 1;
    tbb[1].idCommand = ID_TOOLBAR_OPEN;
    tbb[1].fsState   = TBSTATE_ENABLED;
    tbb[1].fsStyle   = TBSTYLE_BUTTON;
    tbb[1].iString   = (INT_PTR)L"Open";

    tbb[2].iBitmap   = 2;
    tbb[2].idCommand = ID_TOOLBAR_SAVE;
    tbb[2].fsState   = TBSTATE_ENABLED;
    tbb[2].fsStyle   = TBSTYLE_BUTTON;
    tbb[2].iString   = (INT_PTR)L"Save";

    SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hToolbar, TB_ADDBUTTONS, ARRAYSIZE(tbb), (LPARAM)&tbb);

    return hToolbar;
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

    create_main_menu(WM_PRVT->main_window_handler.hwnd);
    create_toolbar(WM_PRVT->main_window_handler.hwnd);

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
