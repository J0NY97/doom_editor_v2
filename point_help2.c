/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_help2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 10:52:31 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 10:52:31 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Is this point a point in any of the point in wall;
*/
int	point_in_wall(t_wall *wall, t_point *point)
{
	return (wall->p1 == point || wall->p2 == point);
}

/*
 * Is this point a point in any of the walls in sector;
*/
int	point_in_sector(t_sector *sector, t_point *point)
{
	t_list	*curr;

	if (!sector || !point)
		return (0);
	curr = sector->walls;
	while (curr)
	{
		if (point_in_wall(curr->content, point))
			return (1);
		curr = curr->next;
	}
	return (0);
}

/*
 *  Check if the point is in any of the list of sectors;
 *  	(must be list of t_sector);
*/
int	point_in_any_sector(t_list *sectors, t_point *point)
{
	while (sectors)
	{
		if (point_in_sector(sectors->content, point))
			return (1);
		sectors = sectors->next;
	}
	return (0);
}

void	remove_all_lonely_points(t_editor *editor)
{
	t_list	*curr;
	t_list	*next;

	curr = editor->points;
	while (curr)
	{
		next = curr->next;
		if (!point_in_any_sector(editor->sectors, curr->content))
			remove_point(editor, curr->content);
		curr = next;
	}
}
