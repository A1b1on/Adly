#ifndef MENU_H
#define MENU_H

#include <Windows.h>
#include "menu_element.h"

typedef struct {
    void* prvt;

    HMENU (*get_menu)(struct menu_t* menu);
    BOOL  (*add_element)(struct menu_t* menu, menu_element_t* menu_element);
} menu_t;

menu_t* create_menu(LPCWSTR menu_name);

#endif // MENU_H