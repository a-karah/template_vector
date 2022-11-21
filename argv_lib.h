#ifndef ARGV_LIB_H
#define ARGV_LIB_H

#include <vector_lib.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct argv_s
{
	int try_condition;
	size_t len;
	size_t capacity;
	vector_t *vector;
	size_t try_index;
} argv_t;

int argv_del_one(argv_t *argv, size_t index, void (*del)(vector_t));
int argv_del_n(argv_t *argv, size_t index, void (*del)(vector_t), size_t n);
int argv_del_all(argv_t *argv, void (*del)(vector_t));

int argv_check_capacity(argv_t *argv, size_t len);
int argv_is_unique(argv_t *argv, vector_t value);
int argv_reverse(argv_t *argv);

int argv_insert(argv_t *argv, size_t index, vector_t value);
int argv_insert_vector_n(argv_t *argv, size_t index, vector_t value[], size_t n);
int argv_push(argv_t *argv, vector_t valueess);
vector_t argv_pull(argv_t *argv);

int argv_try(argv_t *argv, vector_t value, size_t index, int (*fptr)(vector_t, vector_t));
argv_t *argv_splice(argv_t *argv, size_t index, size_t n);
argv_t *argv_new(vector_t vector[], size_t n, vector_t (*fptr)(vector_t));
int argv_destroy(argv_t *argv, void (*del)(vector_t));

#endif
