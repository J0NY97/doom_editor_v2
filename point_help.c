/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 10:51:09 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 10:51:09 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_point	*get_point_with_id(t_list *list, int id)
{
	while (list)
	{
		if (((t_point *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_point	*get_point_from_list_at_pos(t_list *list, t_vec2i v)
{
	t_list	*curr;
	t_point	*p;

	curr = list;
	while (curr)
	{
		p = curr->content;
		if (compare_veci(p->pos.v, v.v, 2))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_point	*get_point_from_wall_at_pos(t_wall *wall, t_vec2i pos)
{
	if (compare_veci(wall->p1->pos.v, pos.v, 2))
		return (wall->p1);
	else if (compare_veci(wall->p2->pos.v, pos.v, 2))
		return (wall->p2);
	return (NULL);
}

t_point	*get_point_from_sector_around_radius(
		t_sector *sector, t_vec2i pos, float allowed_radius)
{
	t_list		*curr;
	t_point		*temp;
	float		x;
	float		y;

	curr = sector->walls;
	while (curr)
	{
		x = -allowed_radius;
		while (x <= allowed_radius)
		{
			y = -allowed_radius;
			while (y <= allowed_radius)
			{
				temp = get_point_from_wall_at_pos(curr->content,
						vec2i(pos.x + (int)x, pos.y + (int)y));
				if (temp)
					return (temp);
				y += 0.5f;
			}
			x += 0.5f;
		}
		curr = curr->next;
	}
	return (NULL);
}
