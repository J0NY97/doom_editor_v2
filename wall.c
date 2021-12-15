/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:38 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 13:35:27 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_wall	*wall_new(void)
{
	t_wall	*wall;

	wall = ft_memalloc(sizeof(t_wall));
	wall->solid = 1;
	wall->wall_texture = 7;
	wall->texture_scale = 30.0f;
	return (wall);
}

/*
 * NOTE:
 * 	this function's job is not to free the points,
 * 		they might be used in some other wall;
*/
void	wall_free(void *w, size_t size)
{
	t_wall	*wall;

	wall = w;
	if (!wall)
		return ;
	ft_lstdel(&wall->sprites, &sprite_free);
	free(wall);
	(void)size;
}

void	wall_render(t_editor *editor, t_wall *wall, Uint32 color)
{
	t_vec2i	p1;
	t_vec2i	p2;

	p1 = conversion(editor, wall->p1->pos);
	p2 = conversion(editor, wall->p2->pos);
	ui_surface_line_draw(editor->drawing_surface, p1, p2, color);
}

/*
 * Removes wall from editor walls;
*/
int	remove_wall(t_editor *editor, t_wall *wall)
{
	if (!wall)
		return (0);
	remove_from_list(&editor->walls, wall);
	remove_wall_from_sector(wall->parent_sector, wall);
	remove_all_wall_sprites(editor, wall);
	free(wall);
	wall = NULL;
	--editor->wall_amount;
	return (1);
}

/*
 * Add wall to editor->walls and returns itself;
*/
t_wall	*add_wall(t_editor *editor)
{
	t_wall	*wall;

	wall = wall_new();
	wall->wall_texture = 7;
	add_to_list(&editor->walls, wall, sizeof(t_wall));
	++editor->wall_amount;
	return (wall);
}
