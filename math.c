#include "editor.h"

int	powi(int n, int pow)
{
	if (pow > 0)
		return (n * powi(n, pow - 1));
	return (1);
}

float	distance(int *v1, int *v2, size_t size)
{
	int		result;
	size_t	i;

	i = -1;
	result = 0;
	while (++i < size)
		result += powi(v1[i] - v2[i], 2);
	return (fabs(sqrt(result)));
}

float	get_ratio(t_vec2i orig_wh, t_vec2i new_wh)
{
	float	ratio_x;
	float	ratio_y;

	ratio_x = new_wh.x / (float)orig_wh.x;
	ratio_y = new_wh.y / (float)orig_wh.y;
	if (ratio_x < ratio_y)
		return (ratio_x);
	return (ratio_y);
}

float	get_ratio_f(t_vec2 orig_wh, t_vec2 new_wh)
{
	float	ratio_x;
	float	ratio_y;

	ratio_x = new_wh.x / orig_wh.x;
	ratio_y = new_wh.y / orig_wh.y;
	if (ratio_x < ratio_y)
		return (ratio_x);
	return (ratio_y);
}
