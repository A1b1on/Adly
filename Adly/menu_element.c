#include "menu_element.h"
#include "menu_element_config.h"

#include <stdlib.h>

#define ME_PRVT ((menu_element_prvt_t*)menu_element->prvt)

// TODO: Add saving ids & names to array
typedef struct {
    HMENU  menu;
    LPWSTR element_name;

} menu_element_prvt_t;

// TODO: Change return type -> add logs
static BOOL menu_element_add_option_impl(menu_element_t* menu_element, UINT_PTR id, LPCWSTR option_name)
{
    return AppendMenu(ME_PRVT->menu, MENU_ELEMENT_OPTION_FLAGS, id, option_name);
}

static BOOL menu_element_add_separator_impl(menu_element_t* menu_element)
{
    return AppendMenu(ME_PRVT->menu, MENU_ELEMENT_SEPARATOR_FLAGS, 0, NULL);
}

static HMENU menu_element_get_element_menu_impl(menu_element_t* menu_element)
{
    return ME_PRVT->menu;
}

static LPCWSTR menu_element_get_element_name_impl(menu_element_t* menu_element)
{
    return ME_PRVT->element_name;
}

// TODO: Add logs
menu_element_t* create_menu_element(LPCWSTR menu_element_name)
{
    size_t menu_element_name_length = 0;

    // Create pointer
    menu_element_t* menu_element = (menu_element_t*)malloc(sizeof(menu_element_t));
    if (menu_element == NULL) {
        return NULL;
    }

    // Create private
    menu_element->prvt = malloc(sizeof(menu_element_prvt_t));
    if (menu_element->prvt == NULL) {
        free(menu_element);
        return NULL;
    }

    if (menu_element_name == NULL)
    {
        free(ME_PRVT);
        free(menu_element);
        return NULL;
    }

    // Copy name
    menu_element_name_length = wcslen(menu_element_name) + 1;
    ME_PRVT->element_name    = (LPWSTR)malloc(menu_element_name_length * sizeof(WCHAR));
    wcscpy_s(ME_PRVT->element_name, menu_element_name_length, menu_element_name);

    // Set functions
    menu_element->add_option       = menu_element_add_option_impl;
    menu_element->add_separator    = menu_element_add_separator_impl;
    menu_element->get_element_name = menu_element_get_element_name_impl;
    menu_element->get_element_menu = menu_element_get_element_menu_impl;

    // TODO: return NULL if cant crate menu
    ME_PRVT->menu = CreateMenu();
 
    return menu_element;
}