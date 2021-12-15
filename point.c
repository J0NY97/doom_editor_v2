/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:16 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 15:13:48 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	point_render(t_editor *editor, t_point *point, Uint32 color)
{
	ui_surface_circle_draw_filled(editor->drawing_surface,
		conversion(editor, point->pos), 3, color);
}

/*
 * Creates new point and adds it to editor->points;
 * Returns itself;
*/
t_point	*add_point(t_editor *editor)
{
	t_point	*point;

	point = ft_memalloc(sizeof(t_point));
	++editor->point_amount;
	add_to_list(&editor->points, point, sizeof(t_point));
	return (point);
}

void	point_free(void *p, size_t size)
{
	t_point	*point;

	point = p;
	if (!point)
		return ;
	free(p);
	(void)size;
}

/*
 * Removes point from all sectors, walls,
 * 	and from anything else that are using this point;
*/
int	remove_point(t_editor *editor, t_point *point)
{
	if (!point)
		return (0);
	remove_from_list(&editor->points, point);
	free(point);
	point = NULL;
	ft_printf("[%s] Done removing point.\n", __FUNCTION__);
	--editor->point_amount;
	return (1);
}

void	draw_points(t_editor *editor, t_list *points, Uint32 color)
{
	while (points)
	{
		point_render(editor, points->content, color);
		points = points->next;
	}
}
