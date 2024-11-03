#include "window_manager.h"

#include "main_window.h"
#include "file_editor.h"
#include "toolbar.h"
#include "menu.h"


#include <CommCtrl.h>
#include <stdlib.h>

#define WM_PRVT ((window_manager_prvt_t*)wm_instance->prvt)

typedef struct {
    main_window_t* main_window;
} window_manager_prvt_t;

// Static variable for the singleton instance
static window_manager_t* wm_instance = NULL;

static HRESULT window_manager_create_menu()
{
    HRESULT         ret        = S_OK;
    menu_t*         menu       = NULL;
    menu_element_t* file_menu  = NULL;
    menu_element_t* edit_menu  = NULL;
    menu_element_t* build_menu = NULL;
    menu_element_t* help_menu  = NULL;

    menu = create_menu(L"Main Menu");
    if (menu == NULL) {
        ret = E_POINTER;
        goto exit;
    }

    file_menu = create_menu_element(L"File");
    if (file_menu == NULL) {
        ret = E_POINTER;
        goto exit;
    }
    file_menu->add_option(file_menu, ID_FILE_NEW, L"New");
    file_menu->add_option(file_menu, ID_FILE_OPEN, L"Open");
    file_menu->add_option(file_menu, ID_FILE_SAVE, L"Save");
    file_menu->add_option(file_menu, ID_FILE_SAVE_AS, L"Save As");
    file_menu->add_separator(file_menu);
    file_menu->add_option(file_menu, ID_FILE_EXIT, L"Exit");

    edit_menu = create_menu_element(L"Edit");
    if (edit_menu == NULL) {
        ret = E_POINTER;
        goto exit;
    }
    edit_menu->add_option(edit_menu, ID_EDIT_UNDO, L"Undo");
    edit_menu->add_option(edit_menu, ID_EDIT_REDO, L"Redo");

    build_menu = create_menu_element(L"Build");
    if (build_menu == NULL) {
        ret = E_POINTER;
        goto exit;
    }
    build_menu->add_option(build_menu, ID_BUILD_COMPILE, L"Compile");
    build_menu->add_option(build_menu, ID_BUILD_RUN, L"Run");

    help_menu = create_menu_element(L"Help");
    if (help_menu == NULL) {
        ret = E_POINTER;
        goto exit;
    }
    help_menu->add_option(help_menu, ID_HELP_ABOUT, L"About");

    HMENU hMenu = CreateMenu();

    menu->add_element(menu, file_menu);
    menu->add_element(menu, edit_menu);
    menu->add_element(menu, build_menu);
    menu->add_element(menu, help_menu);

    WM_PRVT->main_window->set_menu(WM_PRVT->main_window, menu);

exit:
    return ret;
}

static HRESULT window_manager_create_toolbar()
{
    toolbar_t* toolbar = NULL;

    toolbar = create_toolbar();
    if (toolbar == NULL) {
        return E_POINTER;
    }

    toolbar->create(toolbar, WM_PRVT->main_window->get_hwnd(WM_PRVT->main_window));
    toolbar->add_button(toolbar, ID_TOOLBAR_NEW, L"New", 0);
    toolbar->add_button(toolbar, ID_TOOLBAR_OPEN, L"Open", 1);
    toolbar->add_button(toolbar, ID_TOOLBAR_SAVE, L"Save", 2);

    return S_OK;
}

static HRESULT window_manager_create_file_editor()
{
    file_editor_t* file_editor = NULL;

    file_editor = create_file_editor();
    if (file_editor == NULL) {
        return E_POINTER;
    }

    file_editor->create(file_editor, WM_PRVT->main_window->get_hwnd(WM_PRVT->main_window), ID_TEXT_EDITOR);

    return S_OK;
}

void resize_callback_impl(RECT rcClient) 
{
    // Resize Text editor
    //MoveWindow(WM_PRVT->main_window_handler.text_editor, 10, 50, rcClient.right - 20, rcClient.bottom - 60, TRUE);
}

static HRESULT create_main_window_impl(HINSTANCE hInstance)
{
    HRESULT  ret = S_OK;

    // TODO: Add assert NULL macro
    WM_PRVT->main_window = create_main_window(hInstance);
    if (WM_PRVT->main_window == NULL) {
        ret = E_POINTER;
        goto exit;
    }

    // TODO: Add assert ret macro
    ret = WM_PRVT->main_window->create(WM_PRVT->main_window);
    if (ret != S_OK) {
        goto exit;
    }

    ret = window_manager_create_menu();
    if (ret != S_OK) {
        goto exit;
    }

    ret = window_manager_create_toolbar();
    if (ret != S_OK) {
        goto exit;
    }

    ret = window_manager_create_file_editor();
    if (ret != S_OK) {
        goto exit;
    }

    // TODO: move show to class
    ShowWindow(WM_PRVT->main_window->get_hwnd(WM_PRVT->main_window), SW_SHOW);

exit:
    return ret;
}

// Function to access the singleton instance
window_manager_t* window_manager_instance(void)
{
    if (wm_instance == NULL) {
        wm_instance = (window_manager_t*)malloc(sizeof(window_manager_t));
        if (wm_instance == NULL) {
            exit(1);
        }

        wm_instance->prvt = malloc(sizeof(window_manager_prvt_t));
        if (wm_instance->prvt == NULL) {
            free(wm_instance);
            exit(1);
        }

        // Set methods
        wm_instance->create_main_window = create_main_window_impl;
        wm_instance->resize_callback    = resize_callback_impl;
    }

    return wm_instance;
}
