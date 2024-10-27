#ifndef WINDOW_MANAGER_CONFIG_H
#define WINDOW_MANAGER_CONFIG_H

#include <Windows.h>

// Main Window config
LRESULT CALLBACK main_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define MAIN_WINDOW_CLASS_NAME L"MainWindowClass"
#define MAIN_WINDOW_ICON       IDI_APPLICATION
#define MAIN_WINDOW_TITLE      L"Adly"


#endif // WINDOW_MANAGER_CONFIG_H