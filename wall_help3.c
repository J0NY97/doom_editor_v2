/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_help3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 12:54:46 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 12:54:46 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_list	*get_next_wall_node(t_list *list, t_wall *wall)
{
	t_wall	*dest_wall;

	while (list)
	{
		dest_wall = list->content;
		if (dest_wall->p1 == wall->p2)
			return (list);
		list = list->next;
	}
	return (NULL);
}

t_wall	*get_wall_with_id(t_list *list, int id)
{
	while (list)
	{
		if (((t_wall *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_wall	*get_wall_from_list_around_radius(
		t_editor *editor, t_list *list, t_vec2i pos, int allowed_radius)
{
	t_wall		*wall;
	t_vec2i		p1;
	t_vec2i		p2;
	float		dist;

	while (list)
	{
		wall = list->content;
		p1 = conversion(editor, wall->p1->pos);
		p2 = conversion(editor, wall->p2->pos);
		dist = fabs(distance_from_vector_to_line(pos, p1, p2));
		if (dist <= allowed_radius)
			if (vector_in_rect_of_radius(pos, p1, p2, allowed_radius))
				return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_vec2i	get_wall_middle(t_wall *wall)
{
	t_vec2i	middle;

	middle.x = (wall->p1->pos.x + wall->p2->pos.x) / 2;
	middle.y = (wall->p1->pos.y + wall->p2->pos.y) / 2;
	return (middle);
}
