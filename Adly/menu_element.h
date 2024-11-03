#ifndef MENU_ELEMENT_H
#define MENU_ELEMENT_H

#include <Windows.h>

typedef struct {
    void* prvt;

    BOOL (*add_option)(struct menu_element_t* menu_element, UINT_PTR id, LPCWSTR option_name);
    BOOL (*add_separator)(struct menu_element_t* menu_element);

    HMENU   (*get_element_menu)(struct menu_element_t* menu_element);
    LPCWSTR (*get_element_name)(struct menu_element_t* menu_element);
} menu_element_t;

menu_element_t* create_menu_element(LPCWSTR menu_element_name);

#endif // MENU_ELEMENT_H