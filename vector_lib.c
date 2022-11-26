#include <vector_lib.h>

size_t find_max_bit(size_t num)
{
	size_t max;

	max = 0;
	while (num)
	{
		++max;
		num >>= 1;
	}
	return (max);
}

int vector_shift_right(vector_t vector[], size_t len, size_t n)
{
	if (vector == NULL)
		return (-1);
	if (n == 0)
		return (0);
	while (len)
	{
		--len;
		vector[len + n] = vector[len];
	}
	return (0);
}

int vector_shift_left(vector_t vector[], size_t len, size_t n)
{
	size_t i;

	if (vector == NULL || len < n)
		return (-1);
	if (n == 0)
		return (0);
	i = 0;
	while (i < len)
	{
		vector[i] = vector[i + n];
		++i;
	}
	return (0);
}

int vector_destroy(vector_t vector[], void (*del)(vector_t), size_t len)
{
	size_t i;

	if (vector == NULL)
		return (-1);
	i = 0;
	if (del != NULL)
	{
		while (i < len)
		{
			del(vector[i]);
			++i;
		}
	}
	free(vector);
	return (0);
}

int vector_reverse_all(vector_t vector[], size_t len)
{
	size_t i;
	vector_t tmp;

	if (vector == NULL)
		return (-1);
	if (!len)
		return (0);
	i = 0;
	while (i < len / 2)
	{
		tmp = vector[i];
		vector[i] = vector[len - i - 1];
		vector[len - i - 1] = tmp;
		++i;
	}
	return (0);
}

vector_t	*vector_expand(vector_t vector[], size_t expansion_len)
{
	vector_t *tmp;
	size_t i;

	if (vector == NULL)
		return (NULL);
	tmp = malloc(sizeof(vector_t) * (expansion_len));
	if (tmp == NULL)
		return (NULL);
	i = 0;
	while (i < expansion_len)
	{
		tmp[i] = 0;
		++i;
	}
	vector_copy_value_n(tmp, vector, expansion_len);
	return (tmp);
}

vector_t	*vector_copy_value_n(vector_t dest[], vector_t src[], size_t n)
{
	size_t i;

	if (dest == NULL || src == NULL || !n)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		++i;
	}
	return (dest);
}

vector_t	*vector_deep_copy_n(vector_t dest[], vector_t src[], vector_t (*copy)(vector_t), size_t n)
{
	size_t i;

	if (dest == NULL || src == NULL || !n || copy == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dest[i] = copy(src[i]);
		++i;
	}
	return (dest);
}

int vector_del_n(vector_t vector[], size_t len, void (*del)(vector_t), size_t n)
{
	size_t i;

	if (vector == NULL)
		return (-1);
	i = 0;
	if (del == NULL)
	{
		if (len < n)
			return (-1);
		return (vector_shift_left(vector, len, n));
	}
	while (i < n)
		del(vector[i++]);
	return (vector_shift_left(vector, len, i));
}

int vector_insert(vector_t vector[], size_t len, vector_t value)
{
	if (vector == NULL)
		return (-1);
	vector_shift_right(vector, len, 1);
	vector[0] = value;
	return (0);
}

int vector_insert_vector_n(vector_t vector[], size_t len, vector_t value[], size_t n)
{
	if (vector == NULL || !n)
		return (-1);
	vector_shift_right(vector, len, n);
	vector_copy_value_n(vector, value, n);
	return (0);
}
