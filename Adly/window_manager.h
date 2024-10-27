#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "window_manager_config.h"

typedef struct {
    void* prvt;

    // Create Main window method
    HRESULT(*create_main_window)(HINSTANCE hInstance);
} window_manager_t;

// Function to access the singleton instance
window_manager_t* window_manager_instance(void);


#endif // WINDOW_MANAGER_H