/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_help2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 12:51:54 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 12:51:54 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * From 'list' of walls, search for 2 't_wall' with either of its points being
 *	being the point 'p', and save them in 'w1' and 'w2';
*/
void	get_walls_connected_to_point(
	t_list *walls, t_point *p, t_wall **w1, t_wall **w2)
{
	t_wall	*wall;

	while (walls)
	{
		wall = walls->content;
		if (wall->p1 == p || wall->p2 == p)
		{
			if (!*w1)
				*w1 = wall;
			else
				*w2 = wall;
		}
		walls = walls->next;
	}
}

bool	can_you_make_portal_of_this_wall(
		t_list *sector_list, t_sector *part_of_sector, t_wall *wall)
{
	t_sector	*sector;
	t_wall		*temp_wall;

	if (!part_of_sector || !wall)
		return (0);
	while (sector_list)
	{
		sector = sector_list->content;
		if (sector != part_of_sector)
		{
			temp_wall = get_sector_wall_at_pos(sector,
					wall->p1->pos, wall->p2->pos);
			if (temp_wall)
			{
				wall->neighbor = sector;
				temp_wall->neighbor = part_of_sector;
				return (1);
			}
		}
		sector_list = sector_list->next;
	}
	return (0);
}

int	wall_in_any_sector(t_list *sectors, t_wall *wall)
{
	if (!wall)
		return (-1);
	while (sectors)
	{
		if (wall_in_sector(sectors->content, wall))
			return (1);
		sectors = sectors->next;
	}
	return (0);
}

int	wall_in_sector(t_sector *sector, t_wall *wall)
{
	t_list	*curr;

	if (!wall)
		return (-1);
	curr = sector->walls;
	while (curr)
	{
		if (curr->content == wall)
			return (1);
		curr = curr->next;
	}
	return (0);
}

t_wall	*get_connected_wall(t_list *list, t_wall *wall)
{
	t_wall	*dest_wall;

	while (list)
	{
		dest_wall = list->content;
		if (dest_wall->p1 == wall->p2)
			return (dest_wall);
		list = list->next;
	}
	return (NULL);
}
