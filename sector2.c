/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:30:05 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:30:05 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Checking if we have anything in p1 and p2 should be
 * 	checked before this function is called;
*/
void	add_wall_to_sector_at_pos(
		t_editor *editor, t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_wall	*wall;

	wall = add_wall(editor);
	wall->parent_sector = sector;
	wall->p1 = get_point_from_sector_around_radius(sector, p1, 0.0f);
	if (!wall->p1)
	{
		wall->p1 = add_point(editor);
		wall->p1->pos = p1;
	}
	wall->p2 = get_point_from_sector_around_radius(sector, p2, 0.0f);
	if (!wall->p2)
	{
		wall->p2 = add_point(editor);
		wall->p2->pos = p2;
	}
	++sector->wall_amount;
	add_to_list(&sector->walls, wall, sizeof(t_wall));
}

/*
 * NOTE: we only need to move the p1 because
 * 	we sort the list of walls in the sector rendering;
 * 	and if we move both of them, all the walls move double the amount
 * 	they should;
*/
void	move_sector(t_sector *sector, t_vec2i move_amount)
{
	t_list	*wall_list;
	t_wall	*wall;

	wall_list = sector->walls;
	while (wall_list)
	{
		wall = wall_list->content;
		wall->p1->pos = vec2i_add(wall->p1->pos, move_amount);
		wall_list = wall_list->next;
	}
}
