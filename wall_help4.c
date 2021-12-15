/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_help4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 12:55:54 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 12:55:54 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * which == 0 floor;
 * 		== 1 ceiling;
*/
void	remove_wall_list_angles(t_list *list, int which)
{
	t_wall	*wall;

	while (list)
	{
		wall = list->content;
		if (which == 0)
			wall->floor_angle = 0;
		else if (which == 1)
			wall->ceiling_angle = 0;
		list = list->next;
	}
}

/*
 * Removes all walls not a part of a sector;
 * Remove all incomplete walls;
*/
void	remove_all_lonely_walls(t_editor *editor)
{
	t_wall	*wall;
	t_list	*curr;
	t_list	*next;

	curr = editor->walls;
	while (curr)
	{
		wall = curr->content;
		next = curr->next;
		if (!wall_in_any_sector(editor->sectors, wall)
			|| wall->p1 == NULL || wall->p2 == NULL)
			remove_wall(editor, curr->content);
		curr = next;
	}
}

void	split_wall(t_editor *editor, t_sector *sector, t_wall *wall)
{
	t_point	*p1;
	t_point	*p3;

	p1 = wall->p1;
	p3 = add_point(editor);
	p3->pos = get_wall_middle(wall);
	wall->p1 = p3;
	add_wall_to_sector_at_pos(editor, sector, p1->pos, p3->pos);
}

void	draw_walls(t_editor *editor, t_list *walls, Uint32 color)
{
	t_wall	*wall;

	while (walls)
	{
		wall = walls->content;
		wall->width = distance(wall->p1->pos.v, wall->p2->pos.v, 2);
		if (!wall->parent_sector)
			ft_printf("[%s] Error: No parent_sector.\n", __FUNCTION__);
		else
			wall->height = wall->parent_sector->ceiling_height
				- wall->parent_sector->floor_height;
		if (wall->neighbor)
		{
			if (get_sector_wall_at_pos(wall->neighbor,
					wall->p1->pos, wall->p2->pos))
				wall_render(editor, wall, 0xffff0000);
			else
				wall->neighbor = NULL;
		}
		else
			wall_render(editor, wall, color);
		point_render(editor, wall->p1, color);
		walls = walls->next;
	}
}
