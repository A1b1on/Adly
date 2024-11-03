#include "menu.h"
#include "menu_config.h"

#include <stdlib.h>

#define MN_PRVT ((menu_prvt_t*)menu->prvt)

// TODO: Add saving elements pointers
typedef struct {
    HMENU  menu;
    LPWSTR menu_name;

} menu_prvt_t;

// TODO: Change return type -> add logs
static HMENU menu_get_menu_impl(menu_t* menu)
{
    return MN_PRVT->menu;
}

static BOOL menu_add_element_impl(menu_t* menu, menu_element_t* menu_element)
{
    return AppendMenu(MN_PRVT->menu, MENU_MENU_ELEMENT_FLAGS, (UINT_PTR)menu_element->get_element_menu(menu_element), menu_element->get_element_name(menu_element));
}

// TODO: Add logs
menu_t* create_menu(LPCWSTR menu_name)
{
    size_t menu_name_length = 0;

    // Create pointer
    menu_t* menu = (menu_t*)malloc(sizeof(menu_t));
    if (menu == NULL) {
        return NULL;
    }

    // Create private
    menu->prvt = malloc(sizeof(menu_prvt_t));
    if (menu->prvt == NULL) {
        free(menu);
        return NULL;
    }

    if (menu_name == NULL)
    {
        free(MN_PRVT);
        free(menu);
        return NULL;
    }

    // Copy name
    menu_name_length   = wcslen(menu_name) + 1;
    MN_PRVT->menu_name = (LPWSTR)malloc(menu_name_length * sizeof(WCHAR));
    wcscpy_s(MN_PRVT->menu_name, menu_name_length, menu_name);

    // Set functions
    menu->add_element = menu_add_element_impl;
    menu->get_menu    = menu_get_menu_impl;

    // TODO: return NULL if cant crate menu
    MN_PRVT->menu = CreateMenu();

    return menu;
}