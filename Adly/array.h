#ifndef ARRAY_H
#define ARRAY_H

#include <Windows.h>
#include <stdlib.h>

typedef struct {
    void*  prvt;

    HRESULT(*resize)  (struct array_t* array, size_t new_capacity);
    HRESULT(*minimize)(struct array_t* array);

    HRESULT(*push_back)(struct array_t* array, void* element);
    HRESULT(*pop_back) (struct array_t* array);
    HRESULT(*free)     (struct array_t* array);

    void* (*at)      (struct array_t* array, size_t index);
    size_t(*size)    (struct array_t* array);
    size_t(*capacity)(struct array_t* array);
} array_t;

array_t* array_init(size_t element_size, size_t capacity);

#endif // ARRAY_H