#ifndef WINDOW_MANAGER_CONFIG_H
#define WINDOW_MANAGER_CONFIG_H

#include <Windows.h>

// Main Window config
LRESULT CALLBACK main_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define MAIN_WINDOW_CLASS_NAME L"MainWindowClass"
#define MAIN_WINDOW_ICON       IDI_APPLICATION
#define MAIN_WINDOW_TITLE      L"Adly"

// Menu item IDs
#define ID_FILE_NEW      101
#define ID_FILE_OPEN     102
#define ID_FILE_EXIT     103
#define ID_EDIT_UNDO     201
#define ID_EDIT_REDO     202
#define ID_BUILD_COMPILE 301
#define ID_BUILD_RUN     302
#define ID_HELP_ABOUT    401

// Toolbar button IDs
#define ID_TOOLBAR_NEW  501
#define ID_TOOLBAR_OPEN 502
#define ID_TOOLBAR_SAVE 503

#endif // WINDOW_MANAGER_CONFIG_H