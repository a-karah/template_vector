#ifndef tmpv_LIB_H
#define tmpv_LIB_H

#include <vector_lib.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct tmpv_s
{
	int try_condition;
	size_t len;
	size_t capacity;
	vector_t *vector;
	size_t try_index;
} tmpv_t;

int tmpv_del_one(tmpv_t *tmpv, size_t index, void (*del)(vector_t));
int tmpv_del_n(tmpv_t *tmpv, size_t index, void (*del)(vector_t), size_t n);
int tmpv_del_all(tmpv_t *tmpv, void (*del)(vector_t));

int tmpv_check_capacity(tmpv_t *tmpv, size_t len);
int tmpv_is_unique(tmpv_t *tmpv, vector_t value);
int tmpv_reverse(tmpv_t *tmpv);

int tmpv_insert(tmpv_t *tmpv, size_t index, vector_t value);
int tmpv_insert_vector_n(tmpv_t *tmpv, size_t index, vector_t value[], size_t n);
int tmpv_push(tmpv_t *tmpv, vector_t valueess);
vector_t tmpv_pull(tmpv_t *tmpv);

int tmpv_try(tmpv_t *tmpv, vector_t value, size_t index, int (*fptr)(vector_t, vector_t));
tmpv_t *tmpv_splice(tmpv_t *tmpv, size_t index, size_t n);
tmpv_t *tmpv_new(vector_t vector[], size_t n, vector_t (*fptr)(vector_t));
int tmpv_destroy(tmpv_t *tmpv, void (*del)(vector_t));

#endif
