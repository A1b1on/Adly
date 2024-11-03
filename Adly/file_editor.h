#ifndef FILE_EDITOR_H
#define FILE_EDITOR_H

#include <Windows.h>

typedef struct {
    void* prvt;


    HRESULT (*create)(struct file_editor_t* file_editor, HWND parent_hwnd, UINT id);
} file_editor_t;

file_editor_t* create_file_editor();

#endif // FILE_EDITOR_H