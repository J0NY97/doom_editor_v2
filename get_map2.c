/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:08:46 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 15:08:46 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	get_points(t_editor *editor, char **lines, int *i)
{
	char	**args;
	t_point	*point;

	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		point = add_point(editor);
		point->id = ft_atoi(args[0]);
		point->pos.x = ft_atoi(args[1]);
		point->pos.y = ft_atoi(args[2]);
		ft_arraydel(args);
	}
}

void	get_walls(t_editor *editor, char **lines, int *i)
{
	char	**args;
	t_wall	*wall;

	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		wall = add_wall(editor);
		wall->id = ft_atoi(args[0]);
		wall->p1 = get_point_with_id(editor->points, ft_atoi(args[1]));
		wall->p2 = get_point_with_id(editor->points, ft_atoi(args[2]));
		wall->wall_texture = ft_atoi(args[3]);
		if (wall->wall_texture < 0)
		{
			wall->skybox = wall->wall_texture;
			wall->wall_texture = 0;
		}
		wall->portal_texture = ft_atoi(args[4]);
		wall->texture_scale = ft_atof(args[5]);
		wall->solid = ft_atoi(args[6]);
		ft_arraydel(args);
	}
}

/*
 * If no wall with correct id found, we will ignore this sprite;
*/
void	get_sprites(t_editor *editor, char **lines)
{
	t_sprite	*sprite;
	int			wall_id;
	t_wall		*parent_wall;
	int			ggg;

	ggg = -1;
	wall_id = ft_atoi(lines[1]);
	parent_wall = get_wall_with_id(editor->walls, wall_id);
	if (parent_wall)
	{
		sprite = add_sprite(editor);
		sprite->id = ft_atoi(lines[0]);
		sprite->parent = parent_wall;
		sprite->pos.x = ft_atoi(lines[2]);
		sprite->pos.y = ft_atoi(lines[3]);
		sprite->texture_id = ft_atoi(lines[4]);
		if (ft_atoi(lines[5]) != 0)
			sprite->scale = ft_atoi(lines[5]) / 100;
		while (++ggg < SPRITE_TYPE_AMOUNT)
			if (ft_strequ(g_sprite_type[ggg], lines[6]))
				sprite->type = ggg;
		add_to_list(&parent_wall->sprites, sprite, sizeof(t_sprite));
		++parent_wall->sprite_amount;
	}
}

void	get_entity(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_entity	*entity;

	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		entity = add_entity(editor);
		entity->type = get_entity_type(args[1]);
		entity->pos.x = ft_atoi(args[2]);
		entity->pos.y = ft_atoi(args[3]);
		entity->z = ft_atoi(args[4]);
		entity->yaw = ft_atoi(args[5]);
		ft_arraydel(args);
	}
}

void	get_events(t_editor *editor, char **args)
{
	t_event		*event;

	event = add_event(editor);
	event->id = ft_atoi(args[0]);
	event->type = get_event_type(args[1]);
	event->action = get_event_action(args[2]);
	event->pointer_type = TYPE_SPRITE;
	if (event->action == SECTOR || event->action == TYPE_SECTOR)
	{
		event->pointer_type = TYPE_SECTOR;
		event->pointer
			= get_sector_by_id_from_list(editor->sectors, ft_atoi(args[3]));
	}
	else
		event->pointer
			= get_sprite_by_id_from_list(editor->sprites, ft_atoi(args[3]));
	event->sector = ft_strdup(args[4]);
	event->min = ft_atoi(args[5]);
	event->max = ft_atoi(args[6]);
	event->speed = ft_atoi(args[7]);
	update_event_elem(event->elem);
}
