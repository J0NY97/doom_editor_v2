#include "editor.h"

/*
 * Only the map type is usefull here, for the correct tickbox to be ticked;
*/
void	get_map_info(t_editor *editor, char **args)
{
	ft_printf("Getting Map Info. ");
	editor->map_type = 0;
	if (ft_strnequ(args[1], "story", 5))
		editor->map_type = 1;
	ft_printf("Success.\n");
}

void	get_spawn(t_editor *editor, char **lines, int *i)
{
	char	**args;

	ft_printf("Getting Spawn. ");
	args = ft_strsplit(lines[*i + 1], '\t');
	editor->spawn.pos.x = ft_atoi(args[1]);
	editor->spawn.pos.y = ft_atoi(args[2]);
	editor->spawn.z = ft_atoi(args[3]);
	editor->spawn.yaw = ft_atoi(args[4]);
	*i += 2;
	ft_arraydel(args);
	ft_printf("Success.\n");
}

void	get_points(t_editor *editor, char **lines, int *i)
{
	char	**args;
	t_point	*point;

	ft_printf("Getting Points. ");
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
	ft_printf("Success.\n");
}

void	get_walls(t_editor *editor, char **lines, int *i)
{
	char	**args;
	t_wall	*wall;

	ft_printf("Getting Walls. ");
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
	ft_printf("Success.\n");
}

void	get_sprites(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_sprite	*sprite;
	int			wall_id;
	t_wall		*parent_wall;
	int			ggg;

	ft_printf("Getting Sprites. ");
	while (lines[*i])
	{
		*i += 1;
		ggg = -1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		wall_id = ft_atoi(args[1]);
		parent_wall = get_wall_with_id(editor->walls, wall_id);
		if (parent_wall)
		{
			sprite = add_sprite(editor);
			sprite->id = ft_atoi(args[0]);
			sprite->parent = parent_wall;
			sprite->pos.x = ft_atoi(args[2]);
			sprite->pos.x = ft_atoi(args[3]);
			sprite->texture_id = ft_atoi(args[4]);
			if (ft_atoi(args[5]) != 0)
				sprite->scale = 100 / ft_atoi(args[5]);
			while (++ggg < SPRITE_TYPE_AMOUNT)
				if (ft_strequ(g_sprite_type[ggg], args[6]))
					sprite->type = ggg;
			add_to_list(&parent_wall->sprites, sprite, sizeof(t_sprite));
			++parent_wall->sprite_amount;
		}
		else
			ft_printf("[%s] No wall with id %d found, ignoring.\n", __FUNCTION__, wall_id);
		ft_arraydel(args);
	}
	ft_printf("Success.\n");
}

/*
 * search from 'list' walls with 'id' in 'id_str' (delim with ' '(space)) and add to 'sector'->walls;
 * and also settings neighbor ids at the same time, becuase why not;
*/
void	get_sector_walls(t_list *list, char *id_str, char *neighbor_str, t_sector *sector) 
{
	char	**wall_ids;	
	char	**neigh_ids;
	int		i;
	t_wall	*wall;
	int		id;

	i = -1;
	wall_ids = ft_strsplit(id_str, ' ');
	neigh_ids = ft_strsplit(neighbor_str, ' ');
	while (wall_ids[++i])
	{
		id = ft_atoi(wall_ids[i]);
		wall = get_wall_with_id(list, id);
		if (wall)
		{
			wall->parent_sector = sector;
			wall->neighbor_id = ft_atoi(neigh_ids[i]);
			++sector->wall_amount;
			add_to_list(&sector->walls, wall, sizeof(t_wall));
		}
		else
			ft_printf("[%s] Couldnt find wall with id : %d.\n", __FUNCTION__, id);
	}
	ft_arraydel(wall_ids);
}

/*
 * From the sector wall goes throguh all and gets the actual sector from the neighbor id in the walls;
*/
void	get_all_actual_sectors(t_editor *editor)
{
	t_list	*curr_sec;
	t_list	*curr_wall;

	curr_sec = editor->sectors;
	while (curr_sec)
	{
		curr_wall = ((t_sector *)curr_sec->content)->walls;
		while (curr_wall)
		{
			((t_wall *)curr_wall->content)->neighbor =
					get_sector_by_id_from_list(editor->sectors, ((t_wall *)curr_wall->content)->neighbor_id);
			curr_wall = curr_wall->next;
		}
		curr_sec = curr_sec->next;
	}
}

void	get_sectors(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_sector	*sector;

	ft_printf("Getting Sectors. ");
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		sector = add_sector(editor);
		sector->id = ft_atoi(args[0]);
		get_sector_walls(editor->walls, args[1], args[2], sector);
		sector->gravity = ft_atoi(args[3]);
		sector->lighting = ft_atoi(args[4]);
		ft_arraydel(args);
	}
	get_all_actual_sectors(editor);
	ft_printf("Success.\n");
}

void	get_fc(t_editor *editor, char **lines, int *i)
{
	char		**args;
	char		**slopes;
	t_sector	*sector;
	t_wall		*wall;
	int			id;

	ft_printf("Getting F&C. ");
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		id = ft_atoi(args[0]);
		sector = get_sector_by_id_from_list(editor->sectors, id);
		sector->floor_height = ft_atoi(args[1]);
		sector->ceiling_height = ft_atoi(args[2]);
		sector->floor_texture = ft_atoi(args[3]);
		sector->ceiling_texture = ft_atoi(args[4]);
		if (sector->floor_texture < 0) // Skybox
		{
			sector->skybox = sector->floor_texture;
			sector->floor_texture = 0;
			sector->ceiling_texture = 0;
		}
		sector->floor_scale = ft_atof(args[5]);
		sector->ceiling_scale = ft_atof(args[6]);
		slopes = ft_strsplit(args[7], ' ');
		if (ft_atoi(slopes[0]) != 0)
		{
			wall = get_wall_with_id(sector->walls, ft_atoi(slopes[0]));
			if (wall)
				wall->floor_angle = ft_atoi(slopes[1]);
		}
		if (ft_atoi(slopes[2]) != 0)
		{
			wall = get_wall_with_id(sector->walls, ft_atoi(slopes[2]));
			if (wall)
				wall->ceiling_angle = ft_atoi(slopes[3]);
		}
		ft_arraydel(args);
	}
	ft_printf("Success.\n");
}

void	get_entity(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_entity	*entity;
	int			ggg;

	ft_printf("Getting Entitties. ");
	while (lines[*i])
	{
		*i += 1;
		ggg = -1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		entity = add_entity(editor);
		while (++ggg < ENTITY_AMOUNT)
			if (ft_strequ(g_entity_data[ggg].name, args[1]))
				entity->type = ggg + 1;
		entity->pos.x = ft_atoi(args[2]);
		entity->pos.y = ft_atoi(args[3]);
		entity->z = ft_atoi(args[4]);
		entity->yaw = ft_atoi(args[5]);
		ft_arraydel(args);
	}
	ft_printf("Success.\n");
}

void	get_events(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_event		*event;

	ft_printf("Getting Events. ");
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		event = add_event(editor);
		event->id = ft_atoi(args[0]);
		for (int d = 0; d < EVENT_TYPE_AMOUNT - 1; d++)
		{
			if (ft_strequ(args[1], g_event_type[d]))
			{
				event->type = d;
				break ;
			}
		}
		for (int d = 0; d < EVENT_ACTION_AMOUNT; d++)
		{
			if (ft_strequ(args[2], g_event_action[d]))
			{
				event->action = d;
				break ;
			}
		}	
		event->pointer_type = TYPE_SPRITE;
		if (event->action == SECTOR)
		{
			event->pointer_type = TYPE_SECTOR;
			event->pointer = get_sector_by_id_from_list(editor->sectors, ft_atoi(args[3]));
		}
		else
			event->pointer = get_sprite_by_id_from_list(editor->sprites, ft_atoi(args[3]));
		event->sector = ft_strdup(args[4]);
		event->min = ft_atoi(args[5]);
		event->max = ft_atoi(args[6]);
		event->speed = ft_atoi(args[7]);
		ft_arraydel(args);

		update_event_elem(event->elem);
	}
	ft_printf("Success.\n");
}

void	helper_pelper(t_editor *editor, char **lines, int *i, void	(*f)(t_editor *, char **))
{
	char		**args;

	ft_printf("Getting Events. ");
	while (lines[*i])
	{
		*i += 1;
		if (lines[*i][0] == '-')
			break ;
		args = ft_strsplit(lines[*i], '\t');
		f(editor, args);
		ft_arraydel(args);
	}
}

void	get_map(t_editor *editor, char *map)
{
	char	*file_content;
	char	**lines;
	int		i;

	ft_printf("[%s] Getting map <%s>.\n", __FUNCTION__, map);
	file_content = ft_get_file_content(map);
	lines = ft_strsplit(file_content, '\n');
	i = -1;
	while (lines[++i])
	{
		if (ft_strnequ(lines[i], "type:map", 8))
			helper_pelper(editor, lines, &i, &get_map_info);
		else if (ft_strnequ(lines[i], "type:spawn", 10))
			get_spawn(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:point", 10))
			get_points(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:wall", 9))
			get_walls(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:wsprite", 12))
			get_sprites(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:sector", 11))
			get_sectors(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:f&c", 8))
			get_fc(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:entity", 11))
			get_entity(editor, lines, &i);
		else if (ft_strnequ(lines[i], "type:event", 10))
			get_events(editor, lines, &i);
		ft_printf("just printing for the fun of it.\n");
	}
	ft_arraydel(lines);
	ft_strdel(&file_content);
	ft_printf("[%s] Success getting map.\n", __FUNCTION__);
}
