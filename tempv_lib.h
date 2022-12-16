#ifndef tempv_LIB_H
#define tempv_LIB_H

#include "vector_lib.h"
#include <stddef.h>
#include <stdlib.h>

#define GENERATE_TEMP_VECTOR_HEADER(type, BEGIN_NAME)                                                                  \
    GENERATE_VECTOR_HEADER(type, BEGIN_NAME)                                                                           \
    typedef struct BEGIN_NAME##tempv_s                                                                                 \
    {                                                                                                                  \
        int    try_condition;                                                                                          \
        size_t len;                                                                                                    \
        size_t capacity;                                                                                               \
        type  *vector;                                                                                                 \
        size_t try_index;                                                                                              \
    } BEGIN_NAME##tempv_t;                                                                                             \
                                                                                                                       \
    int  BEGIN_NAME##tempv_del_one(BEGIN_NAME##tempv_t *tempv, size_t index, void (*del)(type));                       \
    int  BEGIN_NAME##tempv_del_n(BEGIN_NAME##tempv_t *tempv, size_t index, void (*del)(type), size_t n);               \
    int  BEGIN_NAME##tempv_del_all(BEGIN_NAME##tempv_t *tempv, void (*del)(type));                                     \
                                                                                                                       \
    int  BEGIN_NAME##tempv_check_capacity(BEGIN_NAME##tempv_t *tempv, size_t len);                                     \
    int  BEGIN_NAME##tempv_reverse(BEGIN_NAME##tempv_t *tempv);                                                        \
                                                                                                                       \
    int  BEGIN_NAME##tempv_insert(BEGIN_NAME##tempv_t *tempv, size_t index, type value);                               \
    int  BEGIN_NAME##tempv_insert_vector_n(BEGIN_NAME##tempv_t *tempv, size_t index, type value[], size_t n);          \
    int  BEGIN_NAME##tempv_push(BEGIN_NAME##tempv_t *tempv, type valueess);                                            \
    type BEGIN_NAME##tempv_pull(BEGIN_NAME##tempv_t *tempv);                                                           \
                                                                                                                       \
    int  BEGIN_NAME##tempv_try(BEGIN_NAME##tempv_t *tempv, type value, size_t index, int (*fptr)(type, type));         \
    BEGIN_NAME##tempv_t *BEGIN_NAME##tempv_cut(BEGIN_NAME##tempv_t *tempv, size_t index, size_t n);                 \
    BEGIN_NAME##tempv_t *BEGIN_NAME##tempv_new(type vector[], size_t n, type (*fptr)(type));                           \
    int                  BEGIN_NAME##tempv_destroy(BEGIN_NAME##tempv_t *tempv, void (*del)(type));

#define GENERATE_TEMP_VECTOR(type, BEGIN_NAME)                                                                         \
    GENERATE_VECTOR(type, BEGIN_NAME)                                                                                  \
    int BEGIN_NAME##tempv_destroy(BEGIN_NAME##tempv_t *tempv, void (*del)(type))                                       \
    {                                                                                                                  \
        if (tempv == NULL)                                                                                             \
            return (-1);                                                                                               \
        BEGIN_NAME##vector_destroy(tempv->vector, del, tempv->len);                                                    \
        free(tempv);                                                                                                   \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_try(BEGIN_NAME##tempv_t *tempv, type value, size_t index, int (*fptr)(type, type))           \
    {                                                                                                                  \
        if (tempv == NULL || fptr == NULL || tempv->len <= index)                                                      \
            return (-1);                                                                                               \
        while (index < tempv->len)                                                                                     \
        {                                                                                                              \
            if (fptr(tempv->vector[index], value) == tempv->try_condition)                                             \
            {                                                                                                          \
                tempv->try_index = index;                                                                              \
                return (0);                                                                                            \
            }                                                                                                          \
            ++index;                                                                                                   \
        }                                                                                                              \
        return (-1);                                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    BEGIN_NAME##tempv_t *BEGIN_NAME##tempv_cut(BEGIN_NAME##tempv_t *tempv, size_t index, size_t n)                  \
    {                                                                                                                  \
        BEGIN_NAME##tempv_t *ret;                                                                                      \
                                                                                                                       \
        if (tempv == NULL || n == 0 || tempv->len <= index || tempv->len < index + n || !n)                            \
            return (NULL);                                                                                             \
        ret = BEGIN_NAME##tempv_new(NULL, 0, NULL);                                                                    \
        BEGIN_NAME##tempv_insert_vector_n(ret, 0, &tempv->vector[index], n);                                           \
        BEGIN_NAME##tempv_del_n(tempv, index, NULL, n);                                                                \
        return (ret);                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    BEGIN_NAME##tempv_t *BEGIN_NAME##tempv_new(type vector[], size_t n, type (*fptr)(type))                            \
    {                                                                                                                  \
        BEGIN_NAME##tempv_t *tempv;                                                                                    \
                                                                                                                       \
        tempv = malloc(sizeof(BEGIN_NAME##tempv_t));                                                                   \
        if (tempv == NULL)                                                                                             \
            return (NULL);                                                                                             \
        tempv->try_index     = 0;                                                                                      \
        tempv->try_condition = 0;                                                                                      \
        tempv->len           = n;                                                                                      \
        tempv->capacity      = 0;                                                                                      \
        if (fptr == NULL)                                                                                              \
        {                                                                                                              \
            tempv->vector = BEGIN_NAME##vector_expand(vector, 1 << BEGIN_NAME##find_max_bit(tempv->len));              \
            if (tempv->vector != NULL)                                                                                 \
                tempv->capacity = 1 << BEGIN_NAME##find_max_bit(tempv->len);                                           \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            tempv->vector = BEGIN_NAME##vector_expand(NULL, tempv->capacity);                                          \
            BEGIN_NAME##vector_deep_copy_n(tempv->vector, vector, fptr, n);                                            \
        }                                                                                                              \
        return (tempv);                                                                                                \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_del_one(BEGIN_NAME##tempv_t *tempv, size_t index, void (*del)(type))                         \
    {                                                                                                                  \
        if (tempv == NULL || tempv->len <= index)                                                                      \
            return (-1);                                                                                               \
        if (0 < BEGIN_NAME##vector_del_n(&tempv->vector[index], tempv->len - index, del, 1))                           \
            return (-1);                                                                                               \
        --tempv->len;                                                                                                  \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_del_all(BEGIN_NAME##tempv_t *tempv, void (*del)(type))                                       \
    {                                                                                                                  \
        if (tempv == NULL)                                                                                             \
            return (-1);                                                                                               \
        if (0 < BEGIN_NAME##vector_del_n(tempv->vector, tempv->len, del, tempv->len))                                  \
            return (-1);                                                                                               \
        tempv->len -= 0;                                                                                               \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_del_n(BEGIN_NAME##tempv_t *tempv, size_t index, void (*del)(type), size_t n)                 \
    {                                                                                                                  \
        if (tempv == NULL || tempv->len <= index || tempv->len < index + n || !n)                                      \
            return (-1);                                                                                               \
        if (0 < BEGIN_NAME##vector_del_n(&tempv->vector[index], tempv->len - index, del, n))                           \
            return (-1);                                                                                               \
        tempv->len -= n;                                                                                               \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_insert(BEGIN_NAME##tempv_t *tempv, size_t index, type value)                                 \
    {                                                                                                                  \
        if (tempv == NULL || tempv->len <= index)                                                                      \
            return (-1);                                                                                               \
        BEGIN_NAME##tempv_check_capacity(tempv, tempv->len);                                                           \
        if (0 < BEGIN_NAME##vector_insert(&tempv->vector[index], tempv->len - index, value))                           \
            return (-1);                                                                                               \
        ++tempv->len;                                                                                                  \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_insert_vector_n(BEGIN_NAME##tempv_t *tempv, size_t index, type value[], size_t n)            \
    {                                                                                                                  \
        if (tempv == NULL || value == NULL || tempv->len <= index || !n)                                               \
            return (-1);                                                                                               \
        BEGIN_NAME##tempv_check_capacity(tempv, tempv->len + n);                                                       \
        if (0 < BEGIN_NAME##vector_insert_vector_n(&tempv->vector[index], tempv->len - index, value, n))               \
            return (-1);                                                                                               \
        tempv->len += n;                                                                                               \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_push(BEGIN_NAME##tempv_t *tempv, type value)                                                 \
    {                                                                                                                  \
        if (tempv == NULL)                                                                                             \
            return (-1);                                                                                               \
        BEGIN_NAME##tempv_check_capacity(tempv, tempv->len);                                                           \
        tempv->vector[tempv->len] = value;                                                                             \
        ++tempv->len;                                                                                                  \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    type BEGIN_NAME##tempv_pull(BEGIN_NAME##tempv_t *tempv)                                                            \
    {                                                                                                                  \
        type tmp;                                                                                                      \
                                                                                                                       \
        if (tempv == NULL)                                                                                             \
            return ((type){0});                                                                                        \
        if (tempv->len)                                                                                                \
        {                                                                                                              \
            tmp = tempv->vector[tempv->len - 1];                                                                       \
            --tempv->len;                                                                                              \
            return (tmp);                                                                                              \
        }                                                                                                              \
        return ((type){0});                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_check_capacity(BEGIN_NAME##tempv_t *tempv, size_t len)                                       \
    {                                                                                                                  \
        type  *tmp;                                                                                                    \
        size_t ex_len;                                                                                                 \
                                                                                                                       \
        if (tempv == NULL)                                                                                             \
            return (-1);                                                                                               \
        tmp    = NULL;                                                                                                 \
        ex_len = tempv->capacity;                                                                                      \
        while (ex_len <= len)                                                                                          \
        {                                                                                                              \
            if (!ex_len)                                                                                               \
                ex_len = 1;                                                                                            \
            ex_len *= 2;                                                                                               \
            if (ex_len <= len)                                                                                         \
                continue;                                                                                              \
            tmp = BEGIN_NAME##vector_expand(tempv->vector, ex_len);                                                    \
            if (tmp == NULL)                                                                                           \
                return (-1);                                                                                           \
            tempv->capacity = ex_len;                                                                                  \
            free(tempv->vector);                                                                                       \
            tempv->vector = tmp;                                                                                       \
            break;                                                                                                     \
        }                                                                                                              \
        return (0);                                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    int BEGIN_NAME##tempv_reverse(BEGIN_NAME##tempv_t *tempv)                                                          \
    {                                                                                                                  \
        if (tempv == NULL)                                                                                             \
            return (-1);                                                                                               \
        return (BEGIN_NAME##vector_reverse_all(tempv->vector, tempv->len));                                            \
    }

#endif
