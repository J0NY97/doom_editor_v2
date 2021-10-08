#include "libui.h"

t_vec2	vec2(float x, float y)
{
	t_vec2	v;

	v.x = x;
	v.y = y;
	return (v);
}

t_vec2i	vec2i(int x, int y)
{
	t_vec2i	v;

	v.x = x;
	v.y = y;
	return (v);
}

void	vec2i_swap(t_vec2i *v1, t_vec2i *v2)
{
	t_vec2i	temp;

	temp = *v1;
	for (int i = 0; i < VEC2_SIZE; i++)
		v1->v[i] = v2->v[i];
	for (int i = 0; i < VEC2_SIZE; i++)
		v2->v[i] = temp.v[i];
}

void	vec2_swap(t_vec2 *v1, t_vec2 *v2)
{
	t_vec2	temp;

	temp = *v1;
	for (int i = 0; i < VEC2_SIZE; i++)
		v1->v[i] = v2->v[i];
	for (int i = 0; i < VEC2_SIZE; i++)
		v2->v[i] = temp.v[i];
}


t_vec2i	vec2i_add(t_vec2i v1, t_vec2i v2)
{
	t_vec2i	v;

	v.x = v1.x + v2.x;
	v.y = v1.y + v2.y;
	return (v);
}

t_vec4	vec4(float x, float y, float w, float h)
{
	t_vec4	v;

	v.x = x;
	v.y = y;
	v.w = w;
	v.h = h;
	return (v);
}

t_vec4i	vec4i(int x, int y, int w, int h)
{
	t_vec4i	v;

	v.x = x;
	v.y = y;
	v.w = w;
	v.h = h;
	return (v);
}

void	vec4i_swap(t_vec4i *v1, t_vec4i *v2)
{
	t_vec4i	temp;

	temp = *v1;
	for (int i = 0; i < VEC4_SIZE; i++)
		v1->v[i] = v2->v[i];
	for (int i = 0; i < VEC4_SIZE; i++)
		v2->v[i] = temp.v[i];
}

void	vec4_swap(t_vec4 *v1, t_vec4 *v2)
{
	t_vec4	temp;

	temp = *v1;
	for (int i = 0; i < VEC4_SIZE; i++)
		v1->v[i] = v2->v[i];
	for (int i = 0; i < VEC4_SIZE; i++)
		v2->v[i] = temp.v[i];
}

void	print_vec(float *vec, size_t size)
{
	size_t	i;

	i = -1;
	ft_printf("vec { ");
	while (++i < size)
		ft_printf("%.2f ", vec[i]);
	ft_printf("}\n");
}

void	print_veci(int *vec, size_t size)
{
	size_t	i;

	i = -1;
	ft_printf("vec { ");
	while (++i < size)
		ft_printf("%d ", vec[i]);
	ft_printf("}\n");
}

int	compare_veci(int *vec1, int *vec2, size_t size)
{
	size_t	i;

	i = -1;
	while (++i < size)
	{
		if (vec1[i] != vec2[i])
			return (0);
	}
	return (1);
}

int	compare_vec(float *vec1, float *vec2, size_t size)
{
	size_t	i;

	i = -1;
	while (++i < size)
	{
		if (vec1[i] != vec2[i])
			return (0);
	}
	return (1);
}

int	vec2_in_vec4(t_vec2i v2, t_vec4i v4)
{
	if (v2.x >= v4.x && v2.y >= v4.y
		&& v2.x < v4.w && v2.y < v4.h)
		return (1);
	return (0);
}
