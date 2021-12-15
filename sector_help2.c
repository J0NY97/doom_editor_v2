/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_help2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:33:00 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:33:00 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * If we find from 'list->content->neighbor' (content should be t_wall)...
 * ... the 'sector', we will set 't_wall->neighbor' to NULL. aka remove its
 * neighbor. And also set the wall to solid.
*/
void	remove_neighbor_from_walls(t_list *list, t_sector *sector)
{
	t_wall	*wall;

	while (list)
	{
		wall = list->content;
		if (wall->neighbor == sector)
		{
			wall->neighbor = NULL;
			wall->solid = 1;
		}
		list = list->next;
	}
}

int	get_next_sector_id(t_list *list)
{
	t_sector	*event;
	int			i;
	char		ids[1024];

	i = -1;
	memset(ids, 0, sizeof(char) * 1024);
	while (list)
	{
		event = list->content;
		ids[event->id] = 1;
		list = list->next;
	}
	while (ids[++i])
		;
	return (i);
}

/*
 * 'pc' is the one we are comparing;
*/
int	get_point_side(t_vec2i p1, t_vec2i p2, t_vec2i pc)
{
	return ((p2.x - p1.x) * (pc.y - p1.y) - (p2.y - p1.y) * (pc.x - p1.x));
}

/*
 * Yoinked from here:
 * http://www.sunshine2k.de/coding/java/Polygon/Convex/polygon.htm
 * https://github.com/Epicurius
*/
int	check_sector_convexity(t_sector *sector)
{
	int		i;
	int		res;
	int		newres;
	t_wall	*w1;
	t_wall	*w2;
	t_list	*curr;

	i = 0;
	curr = sector->walls;
	while (curr)
	{
		w1 = curr->content;
		w2 = get_connected_wall(sector->walls, w1);
		if (!w2)
			return (-1);
		if (i == 0)
			res = get_point_side(w1->p2->pos, w2->p2->pos, w1->p1->pos);
		else
		{
			newres = get_point_side(w1->p2->pos, w2->p2->pos, w1->p1->pos);
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
			res = newres;
		}
		i++;
		curr = curr->next;
	}
	return (1);
}

/*
 * The wall should be sorted, winding doesnt matter;
*/
int	check_point_in_sector(t_sector *sector, t_vec2i p)
{
	int		i;
	int		res;
	int		newres;
	t_wall	*wall;
	t_list	*curr;

	i = 0;
	curr = sector->walls;
	while (curr)
	{
		wall = curr->content;
		if (i == 0)
			res = get_point_side(wall->p1->pos, wall->p2->pos, p);
		else
		{
			newres = get_point_side(wall->p1->pos, wall->p2->pos, p);
			if (newres == 0 || res == 0
				|| (newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
			res = newres;
		}
		i++;
		curr = curr->next;
	}
	return (1);
}
