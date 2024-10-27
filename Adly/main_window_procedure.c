#include "window_manager.h"

// Window procedure to handle messages for the main window
LRESULT CALLBACK main_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    window_manager_t* wm = window_manager_instance();

    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_FILE_NEW:
        case ID_FILE_OPEN:
        case ID_FILE_EXIT:
        case ID_EDIT_UNDO:
        case ID_EDIT_REDO:
        case ID_BUILD_COMPILE:
        case ID_BUILD_RUN:
        case ID_HELP_ABOUT:
        case ID_TOOLBAR_NEW:
        case ID_TOOLBAR_OPEN:
        case ID_TOOLBAR_SAVE:
            // Temporary empty
            break;
        }
        break;
    case WM_SIZE: 
        RECT rcClient;
        GetClientRect(hwnd, &rcClient);

        wm->resize_callback(rcClient);
        break;
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}
