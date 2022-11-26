#include <tmpv_lib.h>

int tmpv_destroy(tmpv_t *tmpv, void (*del)(vector_t))
{
	if (tmpv == NULL)
		return (-1);
	vector_destroy(tmpv->vector, del, tmpv->len);
	free(tmpv);
	return (0);
}

int tmpv_try(tmpv_t *tmpv, vector_t value, size_t index, int (*fptr)(vector_t, vector_t))
{
	if (tmpv == NULL || fptr == NULL || tmpv->len <= index)
		return (-1);
	while (index < tmpv->len)
	{
		if (fptr(tmpv->vector[index], value) == tmpv->try_condition)
		{
			tmpv->try_index = index;
			return (0);
		}
		++index;
	}
	return (-1);
}

tmpv_t *tmpv_splice(tmpv_t *tmpv, size_t index, size_t n)
{
	tmpv_t *ret;

	if (tmpv == NULL || n == 0 || tmpv->len <= index || tmpv->len < index + n || !n)
		return (NULL);
	ret = tmpv_new(NULL, 0, NULL);
	tmpv_insert_vector_n(ret, 0, &tmpv->vector[index], n);
	tmpv_del_n(tmpv, index, NULL, n);
	return (ret);
}

tmpv_t *tmpv_new(vector_t vector[], size_t n, vector_t (*fptr)(vector_t))
{
	tmpv_t *tmpv;

	tmpv = malloc(sizeof(tmpv_t));
	if (tmpv == NULL)
		return (NULL);
	tmpv->try_index = 0;
	tmpv->try_condition = 0;
	tmpv->len = n;
	tmpv->capacity = 1 << find_max_bit(tmpv->len);
	if (fptr == NULL)
		tmpv->vector = vector_expand(vector, tmpv->capacity);
	else
	{
		tmpv->vector = vector_expand(NULL, tmpv->capacity);
		vector_deep_copy_n(tmpv->vector, vector, fptr, n);
	}
	return (tmpv);
}

int tmpv_del_one(tmpv_t *tmpv, size_t index, void (*del)(vector_t))
{
	if (tmpv == NULL || tmpv->len <= index)
		return (-1);
	if (0 < vector_del_n(&tmpv->vector[index], tmpv->len - index, del, 1))
		return (-1);
	--tmpv->len;
	return (0);
}

int tmpv_del_all(tmpv_t *tmpv, void (*del)(vector_t))
{
	if (tmpv == NULL)
		return (-1);
	if (0 < vector_del_n(tmpv->vector, tmpv->len, del, tmpv->len))
		return (-1);
	tmpv->len -= 0;
	return (0);
}

int tmpv_del_n(tmpv_t *tmpv, size_t index, void (*del)(vector_t), size_t n)
{
	if (tmpv == NULL || tmpv->len <= index || tmpv->len < index + n || !n)
		return (-1);
	if (0 < vector_del_n(&tmpv->vector[index], tmpv->len - index, del, n))
		return (-1);
	tmpv->len -= n;
	return (0);
}

int tmpv_insert(tmpv_t *tmpv, size_t index, vector_t value)
{
	if (tmpv == NULL || tmpv->len <= index)
		return (-1);
	tmpv_check_capacity(tmpv, tmpv->len + 1);
	if (0 < vector_insert(&tmpv->vector[index], tmpv->len - index, value))
		return (-1);
	++tmpv->len;
	return (0);
}

int tmpv_insert_vector_n(tmpv_t *tmpv, size_t index, vector_t value[], size_t n)
{
	if (tmpv == NULL || value == NULL || tmpv->len <= index || !n)
		return (-1);
	tmpv_check_capacity(tmpv, tmpv->len + n);
	if (0 < vector_insert_vector_n(&tmpv->vector[index], tmpv->len - index, value, n))
		return (-1);
	tmpv->len += n;
	return (0);
}

int tmpv_push(tmpv_t *tmpv, vector_t value)
{
	if (tmpv == NULL)
		return (-1);
	tmpv_check_capacity(tmpv, tmpv->len + 1);
	tmpv->vector[tmpv->len] = value;
	++tmpv->len;
	return (0);
}

vector_t tmpv_pull(tmpv_t *tmpv)
{
	vector_t tmp;

	if (tmpv == NULL)
		return (NULL);
	if (tmpv->len)
	{
		tmp = tmpv->vector[tmpv->len - 1];
		--tmpv->len;
		return (tmp);
	}
	return (0);
}

int tmpv_is_unique(tmpv_t *tmpv, vector_t value)
{
	size_t i;

	if (tmpv == NULL)
		return (0);
	i = 0;
	while (i < tmpv->len)
		if (tmpv->vector[i++] == value)
			return (0);
	return (1);
}

int tmpv_check_capacity(tmpv_t *tmpv, size_t len)
{
	vector_t *tmp;
	size_t ex_len;

	if (tmpv == NULL || !len)
		return (-1);
	tmp = NULL;
	ex_len = tmpv->capacity;
	while (ex_len <= len)
	{
		ex_len *= 2;
		if (ex_len <= len)
			continue;
		tmp = vector_expand(tmpv->vector, ex_len);
		if (tmp == NULL)
			return (-1);
		tmpv->capacity = ex_len;
		free(tmpv->vector);
		tmpv->vector = tmp;
		break;
	}
	return (0);
}

int tmpv_reverse(tmpv_t *tmpv)
{
	if (tmpv == NULL)
		return (-1);
	return (vector_reverse_all(tmpv->vector, tmpv->len));
}
