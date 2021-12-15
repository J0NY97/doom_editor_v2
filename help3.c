/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:09:51 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 14:09:51 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Returns the t_list of which t_list->content == 'content';
*/
t_list	*get_tlist_with_content(t_list *list, void *content)
{
	while (list)
	{
		if (list->content == content)
			return (list);
		list = list->next;
	}
	return (NULL);
}

void	remove_from_list(t_list **list, void *pointer)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (curr->content == pointer)
			ft_lstdelone_nonfree(list, curr);
		curr = curr->next;
	}
}

float	distance_from_vector_to_line(t_vec2i p0, t_vec2i p1, t_vec2i p2)
{
	float		dist;
	float		up;
	float		down;

	up = (p2.x - p1.x) * (p1.y - p0.y)
		- (p1.x - p0.x) * (p2.y - p1.y);
	down = sqrt(ft_pow(p2.x - p1.x, 2) + ft_pow(p2.y - p1.y, 2));
	dist = up / down;
	return (dist);
}

int	vector_in_rect_of_radius(t_vec2i p, t_vec2i v1, t_vec2i v2, float radius)
{
	float	min;
	float	max;

	min = fmin(v1.x, v2.x);
	max = fmax(v1.x, v2.x);
	if (p.x >= min - radius
		&& p.x <= max + radius)
	{
		min = fmin(v1.y, v2.y);
		max = fmax(v1.y, v2.y);
		if (p.y >= min - radius
			&& p.y <= max + radius)
			return (1);
	}
	return (0);
}
