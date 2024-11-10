#include <windows.h>
#include "window_manager.h"
#include "log_manager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    HRESULT ret = S_OK;
    // Get the singleton instance of the WindowManager
    window_manager_t* wm = window_manager_instance();
    LOG_INFO("%s", "Tests");
    // Initialize the main window
    ret = wm->create_main_window(hInstance);
    if (ret != S_OK) {
        return -1;
    }

    // Message loop
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    log_manager_cleanup();

    return (int)msg.wParam;
}
