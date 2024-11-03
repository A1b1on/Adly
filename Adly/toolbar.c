#include "toolbar.h"
#include "toolbar_config.h"

#include <CommCtrl.h>
#include <stdlib.h>

#define TB_PRVT ((toolbar_prvt_t*)toolbar->prvt)

typedef struct {
    HWND hwnd;
} toolbar_prvt_t;

// TODO: Set automatically bitmap
static BOOL toolbar_add_button_impl(toolbar_t* toolbar, UINT id, LPCWSTR button_text, int bitmap_index)
{
    TBBUTTON button  = { 0 };
    button.iBitmap   = bitmap_index;
    button.idCommand = id;
    button.fsState   = TOOLBAR_DEFAULT_STATE;
    button.fsStyle   = TOOLBAR_DEFAULT_STYLE;
    button.iString   = (INT_PTR)button_text;

    SendMessage(TB_PRVT->hwnd, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    return SendMessage(TB_PRVT->hwnd, TB_ADDBUTTONS, 1, (LPARAM)&button);
}

static HWND toolbar_get_hwnd_impl(toolbar_t* toolbar)
{
    return TB_PRVT->hwnd;
}

static HRESULT toolbar_create_impl(toolbar_t* toolbar, HWND parent_hwnd)
{
    TB_PRVT->hwnd = CreateWindowEx(
        0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_TOOLTIPS,
        0, 0, 0, 0, parent_hwnd, NULL, GetModuleHandle(NULL), NULL
    );

    if (!TB_PRVT->hwnd) {
        return E_HANDLE;
    }

    // Standart image for toolbar
    TBADDBITMAP tbBitmap = { HINST_COMMCTRL, IDB_STD_SMALL_COLOR };
    SendMessage(TB_PRVT->hwnd, TB_ADDBITMAP, 0, (LPARAM)&tbBitmap);

    return S_OK;
}

toolbar_t* create_toolbar()
{
    toolbar_t* toolbar = (toolbar_t*)malloc(sizeof(toolbar_t));
    if (!toolbar) {
        return NULL;
    }

    toolbar->prvt = malloc(sizeof(toolbar_prvt_t));
    if (!toolbar->prvt) {
        free(toolbar);
        return NULL;
    }

    toolbar->create     = toolbar_create_impl;
    toolbar->add_button = toolbar_add_button_impl;
    toolbar->get_hwnd   = toolbar_get_hwnd_impl;

    return toolbar;
}
