#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <Windows.h>


typedef struct {
    void* prvt;

    HRESULT(*create)(struct toolbar_t* toolbar, HWND parent_hwnd);
    BOOL   (*add_button)(struct toolbar_t* toolbar, UINT id, LPCWSTR button_text, int bitmap_index);
    HWND   (*get_hwnd)(struct toolbar_t* toolbar);
} toolbar_t;

toolbar_t* create_toolbar();

#endif // TOOLBAR_H