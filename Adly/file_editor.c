#include "file_editor.h"
#include "file_editor_config.h"

#include <stdlib.h>

#define FE_PRVT ((file_editor_prvt_t*)file_editor->prvt)

// TODO: add position & callback 
typedef struct {
    HWND hwnd;
} file_editor_prvt_t;

static HRESULT file_editor_create_impl(file_editor_t* file_editor, HWND parent_hwnd, UINT id)
{
    FE_PRVT->hwnd = CreateWindowEx(
        0, FILE_EDITOR_DEFAULT_CLASS_NAME, NULL,
        FILE_EDITOR_DEFAULT_STYLE,
        10, 50, 400, 300,
        parent_hwnd, (HMENU)id, GetModuleHandle(NULL), NULL);

    if (FE_PRVT->hwnd == NULL) {
        return E_FAIL;
    }
    
    return S_OK;
}

file_editor_t* create_file_editor()
{
    file_editor_t* file_editor = (file_editor_t*)malloc(sizeof(file_editor_t));
    if (!file_editor) {
        return NULL;
    }

    file_editor->prvt = malloc(sizeof(file_editor_prvt_t));
    if (!file_editor->prvt) {
        free(file_editor);
        return NULL;
    }

    file_editor->create = file_editor_create_impl;

    return file_editor;
}
