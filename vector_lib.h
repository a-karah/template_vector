#ifndef VECTOR_LIB_H
#define VECTOR_LIB_H
#include <stdlib.h>
#include <stddef.h>

typedef void *vector_t;

size_t find_max_bit(size_t num);
int vector_shift_right(vector_t vector[], size_t len, size_t n);
int vector_shift_left(vector_t vector[], size_t len, size_t n);

int vector_destroy(vector_t vector[], void (*del)(vector_t), size_t len);
int vector_del_n(vector_t vector[], size_t len, void (*del)(vector_t), size_t n);

void **vector_copy_value_n(vector_t dest[], vector_t src[], size_t n);
void **vector_deep_copy_n(vector_t dest[], vector_t src[], vector_t (*copy)(vector_t), size_t n);

void **vector_expand(vector_t vector[], size_t expansion_len);
int vector_insert(vector_t vector[], size_t len, vector_t value);
int vector_insert_vector_n(vector_t vector[], size_t len, vector_t value[], size_t n);

int vector_reverse_all(vector_t vector[], size_t len);

#endif
