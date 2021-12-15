/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:32:22 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:32:22 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_sector	*get_sector_with_id(t_list *sectors, int id)
{
	while (sectors)
	{
		if (((t_sector *)sectors->content)->id == id)
			return (sectors->content);
		sectors = sectors->next;
	}
	return (NULL);
}

t_vec2i	get_sector_center(t_sector *sector)
{
	int		i;
	float	x;
	float	y;
	t_list	*wall;
	t_wall	*w;

	x = 0;
	y = 0;
	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		x += (w->p1->pos.x + w->p2->pos.x);
		y += (w->p1->pos.y + w->p2->pos.y);
		wall = wall->next;
	}
	i = ft_lstlen(sector->walls) * 2;
	if (i < 2)
		return (vec2i(-1, -1));
	return (vec2i(x / i, y / i));
}

t_sector	*get_sector_around_radius(
		t_editor *editor, t_vec2i pos, int allowed_radius)
{
	t_list		*list;
	t_sector	*sec;
	t_vec2i		screen_pos;

	list = editor->sectors;
	while (list)
	{
		sec = list->content;
		screen_pos = conversion(editor, sec->center);
		if (vec2_in_vec4(pos,
				vec4i(screen_pos.x - allowed_radius,
					screen_pos.y - allowed_radius,
					allowed_radius * 2, allowed_radius * 2)))
			return (sec);
		list = list->next;
	}
	return (NULL);
}

t_sector	*get_sector_by_id_from_list(t_list *list, int id)
{
	while (list)
	{
		if (((t_sector *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_wall	*get_sector_wall_at_pos(t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_list	*curr;
	t_wall	*curr_wall;

	curr = sector->walls;
	while (curr)
	{
		curr_wall = curr->content;
		if ((compare_veci(curr_wall->p1->pos.v, p1.v, 2)
				&& compare_veci(curr_wall->p2->pos.v, p2.v, 2))
			|| (compare_veci(curr_wall->p1->pos.v, p2.v, 2)
				&& compare_veci(curr_wall->p2->pos.v, p1.v, 2)))
			return (curr_wall);
		curr = curr->next;
	}
	return (NULL);
}
