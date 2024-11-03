#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <Windows.h>

#include "menu.h"


typedef struct {
    void* prvt;

    HRESULT (*create)(struct main_window_t* main_window);
    BOOL    (*set_menu)(struct main_window_t* main_window, menu_t* menu);
    HWND    (*get_hwnd)(struct main_window_t* main_window);
} main_window_t;

main_window_t* create_main_window(HINSTANCE hInstance);

#endif // MAIN_WINDOW_H