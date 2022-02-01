/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:41:51 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/17 16:22:10 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*set_points(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_point	*point;
	int		id;

	id = -1;
	final = ft_sprintf("type:point\n");
	curr = editor->points;
	while (curr)
	{
		point = curr->content;
		point->id = ++id;
		temp = ft_sprintf("%d\t%d\t%d\n",
				point->id, point->pos.x, point->pos.y);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*create_wall_str(t_wall *wall)
{
	char	*final;
	int		texture_id;

	texture_id = wall->wall_texture;
	if (wall->parent_sector->skybox != 0)
		texture_id = wall->parent_sector->skybox;
	if (wall->skybox != 0)
		texture_id = wall->skybox;
	final = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.2f\t%d\n",
			wall->id, wall->p1->id, wall->p2->id,
			texture_id, wall->portal_texture,
			wall->texture_scale, wall->solid);
	return (final);
}

char	*set_walls(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_wall	*wall;
	int		id;

	id = -1;
	final = ft_sprintf("type:wall\tp1\tp2\twtx\tptx\tscale\tsolid\n");
	curr = editor->walls;
	while (curr)
	{
		wall = curr->content;
		wall->id = ++id;
		temp = create_wall_str(wall);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*get_wall_sprites(t_wall *wall)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_sprite	*sprite;

	final = NULL;
	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				sprite->id, wall->id, (int)sprite->pos.x, (int)sprite->pos.y,
				sprite->texture_id, (int)(sprite->scale * 100),
				g_sprite_type[sprite->type]);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_sprites(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;

	final = ft_sprintf("type:wsprite\twall\tx\ty\ttexture\tscale\ttype\n");
	curr = editor->walls;
	while (curr)
	{
		temp = get_wall_sprites(curr->content);
		if (temp)
		{
			ft_stradd(&final, temp);
			ft_strdel(&temp);
		}
		curr = curr->next;
	}
	return (final);
}
