#include <tempv_lib.h>

int tempv_destroy(tempv_t *tempv, void (*del)(vector_t))
{
	if (tempv == NULL)
		return (-1);
	vector_destroy(tempv->vector, del, tempv->len);
	free(tempv);
	return (0);
}

int tempv_try(tempv_t *tempv, vector_t value, size_t index, int (*fptr)(vector_t, vector_t))
{
	if (tempv == NULL || fptr == NULL || tempv->len <= index)
		return (-1);
	while (index < tempv->len)
	{
		if (fptr(tempv->vector[index], value) == tempv->try_condition)
		{
			tempv->try_index = index;
			return (0);
		}
		++index;
	}
	return (-1);
}

tempv_t *tempv_splice(tempv_t *tempv, size_t index, size_t n)
{
	tempv_t *ret;

	if (tempv == NULL || n == 0 || tempv->len <= index || tempv->len < index + n || !n)
		return (NULL);
	ret = tempv_new(NULL, 0, NULL);
	tempv_insert_vector_n(ret, 0, &tempv->vector[index], n);
	tempv_del_n(tempv, index, NULL, n);
	return (ret);
}

tempv_t *tempv_new(vector_t vector[], size_t n, vector_t (*fptr)(vector_t))
{
	tempv_t *tempv;

	tempv = malloc(sizeof(tempv_t));
	if (tempv == NULL)
		return (NULL);
	tempv->try_index = 0;
	tempv->try_condition = 0;
	tempv->len = n;
	tempv->capacity = 1 << find_max_bit(tempv->len);
	if (fptr == NULL)
		tempv->vector = vector_expand(vector, 1 << find_max_bit(tempv->len));
	else
	{
		tempv->vector = vector_expand(NULL, tempv->capacity);
		vector_deep_copy_n(tempv->vector, vector, fptr, n);
	}
	return (tempv);
}

int tempv_del_one(tempv_t *tempv, size_t index, void (*del)(vector_t))
{
	if (tempv == NULL || tempv->len <= index)
		return (-1);
	if (0 < vector_del_n(&tempv->vector[index], tempv->len - index, del, 1))
		return (-1);
	--tempv->len;
	return (0);
}

int tempv_del_all(tempv_t *tempv, void (*del)(vector_t))
{
	if (tempv == NULL)
		return (-1);
	if (0 < vector_del_n(tempv->vector, tempv->len, del, tempv->len))
		return (-1);
	tempv->len -= 0;
	return (0);
}

int tempv_del_n(tempv_t *tempv, size_t index, void (*del)(vector_t), size_t n)
{
	if (tempv == NULL || tempv->len <= index || tempv->len < index + n || !n)
		return (-1);
	if (0 < vector_del_n(&tempv->vector[index], tempv->len - index, del, n))
		return (-1);
	tempv->len -= n;
	return (0);
}

int tempv_insert(tempv_t *tempv, size_t index, vector_t value)
{
	if (tempv == NULL || tempv->len <= index)
		return (-1);
	tempv_check_capacity(tempv, tempv->len);
	if (0 < vector_insert(&tempv->vector[index], tempv->len - index, value))
		return (-1);
	++tempv->len;
	return (0);
}

int tempv_insert_vector_n(tempv_t *tempv, size_t index, vector_t value[], size_t n)
{
	if (tempv == NULL || value == NULL || tempv->len <= index || !n)
		return (-1);
	tempv_check_capacity(tempv, tempv->len + n);
	if (0 < vector_insert_vector_n(&tempv->vector[index], tempv->len - index, value, n))
		return (-1);
	tempv->len += n;
	return (0);
}

int tempv_push(tempv_t *tempv, vector_t value)
{
	if (tempv == NULL)
		return (-1);
	tempv_check_capacity(tempv, tempv->len);
	tempv->vector[tempv->len] = value;
	++tempv->len;
	return (0);
}

vector_t tempv_pull(tempv_t *tempv)
{
	vector_t tmp;

	if (tempv == NULL)
		return (NULL);
	if (tempv->len)
	{
		tmp = tempv->vector[tempv->len - 1];
		--tempv->len;
		return (tmp);
	}
	return (0);
}

int tempv_is_unique(tempv_t *tempv, vector_t value)
{
	size_t i;

	if (tempv == NULL)
		return (0);
	i = 0;
	while (i < tempv->len)
		if (tempv->vector[i++] == value)
			return (0);
	return (1);
}

int tempv_check_capacity(tempv_t *tempv, size_t len)
{
	vector_t *tmp;
	size_t ex_len;

	if (tempv == NULL || !len)
		return (-1);
	tmp = NULL;
	ex_len = tempv->capacity;
	while (ex_len <= len)
	{
		ex_len *= 2;
		if (ex_len <= len)
			continue;
		tmp = vector_expand(tempv->vector, ex_len);
		if (tmp == NULL)
			return (-1);
		tempv->capacity = ex_len;
		free(tempv->vector);
		tempv->vector = tmp;
		break;
	}
	return (0);
}

int tempv_reverse(tempv_t *tempv)
{
	if (tempv == NULL)
		return (-1);
	return (vector_reverse_all(tempv->vector, tempv->len));
}
