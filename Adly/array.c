#include "array.h"

#include "log_manager.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define AR_CAPACITY_ENCREASE_COF 2

#define AR_PRVT     ((array_prvt_t*)array->prvt)
#define AR_LAST_PTR (((uint8_t*)(AR_PRVT->start)) + ((AR_PRVT->size - 1) * AR_PRVT->element_size))

typedef struct {
    void*  start;         // Pointer to the start of the array
    size_t size;          // Current number of elements in the array
    size_t capacity;      // Total capacity of the array
    size_t element_size;  // Size of each element in the array
} array_prvt_t;

// Resizes the array to a new capacity
static HRESULT array_resize_impl(array_t* array, size_t new_capacity)
{
    void* temp_array_start = NULL;

    if (array == NULL) {
        LOG_DEBUG("array_resize_impl: Array is NULL");
        return E_POINTER;
    }

    if (new_capacity < AR_PRVT->size || new_capacity == 0) {
        LOG_DEBUG("array_resize_impl: Wrong capacity for array_resize");
        return E_INVALIDARG;
    }

    temp_array_start = realloc(AR_PRVT->start, new_capacity * AR_PRVT->element_size);
    if (temp_array_start == NULL) {
        LOG_ERROR("array_resize_impl: Failed to reallocate memory for array");
        return E_FAIL;
    }

    AR_PRVT->start    = temp_array_start;
    AR_PRVT->capacity = new_capacity;

    return S_OK;
}

// Minimizes the array's capacity to fit its current size
static HRESULT array_minimize_impl(array_t* array)
{
    void* temp_array_start = NULL;

    if (array == NULL) {
        LOG_DEBUG("array_minimize_impl: Array is NULL");
        return E_POINTER;
    }

    if (AR_PRVT->size == AR_PRVT->capacity) {
        LOG_DEBUG("array_minimize_impl: Already minimized");
        return S_OK;
    }

    return array->resize(array, AR_PRVT->size);
}

// Adds an element to the end of the array
static HRESULT array_push_back_impl(array_t* array, void* element)
{
    HRESULT  ret = S_OK;

    if (array == NULL) {
        LOG_DEBUG("array_push_back_impl: Array is NULL");
        ret = E_POINTER;
        goto exit;
    }

    if (element == NULL) {
        LOG_DEBUG("array_push_back_impl: Array element is NULL");
        ret = E_POINTER;
        goto exit;
    }

    if (AR_PRVT->capacity == 0) {
        ret = array->resize(array, AR_CAPACITY_ENCREASE_COF);
        if (ret != S_OK) {
            goto exit;
        }
    }

    if (AR_PRVT->size == AR_PRVT->capacity) {
        ret = array->resize(array, AR_PRVT->capacity * AR_CAPACITY_ENCREASE_COF);
        if (ret != S_OK) {
            goto exit;
        }
    }

    memcpy(AR_LAST_PTR + AR_PRVT->element_size, element, AR_PRVT->element_size);

    AR_PRVT->size += 1;

exit:
    return ret;
}

// Removes the last element from the array
static HRESULT array_pop_back_impl(array_t* array)
{
    HRESULT ret = S_OK;

    if (array == NULL) {
        LOG_DEBUG("array_pop_back_impl: Array is NULL");
        ret = E_POINTER;
        goto exit;
    }

    if (AR_PRVT->size == 0) {
        goto exit;
    }

    memset(AR_LAST_PTR, 0, AR_PRVT->element_size);

    AR_PRVT->size -= 1;

exit:
    return ret;
}

// Frees the memory allocated for the array
static HRESULT array_free_impl(array_t* array)
{
    HRESULT ret = S_OK;

    if (array == NULL) {
        LOG_DEBUG("array_free_impl: Array is NULL");
        ret = E_POINTER;
        goto exit;
    }

    if (AR_PRVT == 0) {
        goto exit;
    }

    free(AR_PRVT->start);
    AR_PRVT->size     = 0;
    AR_PRVT->capacity = 0;

exit:
    return ret;
}

// Retrieves the element at the specified index
static void* array_at_impl(array_t* array, size_t index)
{
    if (array == NULL) {
        LOG_DEBUG("array_at_impl: Array is NULL");
        return NULL;
    }
    if (index >= AR_PRVT->size) {
        return NULL;
    }

    return (void*)(((uint8_t*)(AR_PRVT->start)) + (index * AR_PRVT->element_size));
}

// Returns the current size of the array
static size_t array_size_impl(array_t* array)
{
    if (array == NULL) {
        LOG_DEBUG("array_size_impl: Array is NULL");
        return 0;
    }

    return AR_PRVT->size;
}

// Returns the current capacity of the array
static size_t array_capacity_impl(array_t* array)
{
    if (array == NULL) {
        LOG_DEBUG("array_capacity_impl: Array is NULL");
        return 0;
    }

    return AR_PRVT->capacity;
}

// Initializes a new array with the specified element size and capacity
array_t* array_init(size_t element_size, size_t capacity)
{
    array_t* array = (array_t*)malloc(sizeof(array_t));
    if (array == NULL) {
        LOG_ERROR("array_init: Failed to allocate memory for array");
        goto exit;
    }

    AR_PRVT = (array_prvt_t*)malloc(sizeof(array_prvt_t));
    if (AR_PRVT == NULL) {
        LOG_ERROR("array_init: Failed to allocate memory for array");
        goto clean;
    }

    if (element_size == 0) {
        LOG_ERROR("array_init: Failed array init, element size = 0");
        goto full_clean;
    }

    AR_PRVT->element_size = element_size;

    if (capacity != 0) {
        AR_PRVT->start = malloc(capacity * AR_PRVT->element_size);
        if (AR_PRVT->start == NULL) {
            LOG_ERROR("array_init: Failed to allocate array");
            goto full_clean;
        }
        memset(AR_PRVT->start, 0, capacity * AR_PRVT->element_size);
    }

    AR_PRVT->size     = 0;
    AR_PRVT->capacity = capacity;

    array->resize    = array_resize_impl;
    array->minimize  = array_minimize_impl;
    array->push_back = array_push_back_impl;
    array->pop_back  = array_pop_back_impl;
    array->free      = array_free_impl;
    array->at        = array_at_impl;
    array->size      = array_size_impl;
    array->capacity  = array_capacity_impl;

    return array;

full_clean:
    free(AR_PRVT);
clean:
    free(array);
exit:
    return NULL;
}
