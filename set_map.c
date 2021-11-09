#include "editor.h"

char	*set_map_info(t_editor *editor)
{
	char	*final;
	char	*map_type;

	if (editor->map_type == 1)
		map_type = ft_strdup("story");
	else
		map_type = ft_strdup("endless");
	final = ft_sprintf("type:map\ttype\tscale\tvert\twall\tsect\n%d\t%s\t%d\t%d\t%d\t%d\n",
			0,
			map_type,
			1,
			ft_lstlen(editor->points),
			ft_lstlen(editor->walls),
			ft_lstlen(editor->sectors));
	ft_strdel(&map_type);
	return (final);
}

char	*set_spawn(t_editor *editor)
{
	char	*final;

	final = ft_sprintf("type:spawn\tx\ty\tz\tyaw\n%d\t%d\t%d\t%d\t%d\n",
			0,
			editor->spawn.pos.x,
			editor->spawn.pos.y,
			editor->spawn.z,
			editor->spawn.yaw);
	return (final);
}

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
		temp = ft_sprintf("%d\t%d\t%d\n", point->id, point->pos.x, point->pos.y);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
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
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.2f\t%d\n",
				wall->id, wall->p1->id, wall->p2->id,
				wall->wall_texture, wall->portal_texture,
				wall->texture_scale, wall->solid);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*get_wall_sprites(t_wall *wall, int *id)
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
		*id += 1;
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%d\t%s\n",
				*id, wall->id, sprite->pos.x, sprite->pos.y,
				sprite->texture, (int)(sprite->scale * 100), g_sprite_type[sprite->type]);
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
	int		total_sprites;

	total_sprites = -1;
	final = ft_sprintf("type:wsprite\twall\tx\ty\ttexture\tscale\ttype\n");
	curr = editor->walls;
	while (curr)
	{
		temp = get_wall_sprites(curr->content, &total_sprites);
		if (temp)
		{
			ft_stradd(&final, temp);
			ft_strdel(&temp);
		}
		curr = curr->next;
	}
	return (final);
}

char	*get_sector_wall_ids(t_sector *sector)
{
	char	*final;
	char	*temp;
	char	*neighbors;
	t_list	*curr;
	t_wall	*wall;

	final = NULL;
	neighbors = NULL;
	curr = sector->walls;
	while (curr)
	{
		wall = curr->content;
		temp = ft_sprintf("%d ", wall->id);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		if (wall->neighbor)
			temp = ft_sprintf("%d ", wall->neighbor->id);
		else
			temp = ft_sprintf("%d ", -1);
		ft_stradd(&neighbors, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	ft_stradd(&final, "\t");
	ft_stradd(&final, neighbors);
	ft_strdel(&neighbors);
	return (final);
}

char	*set_sectors(t_editor *editor)
{
	char		*final;
	char		*temp;
	char		*walls;
	t_list		*curr;
	t_sector	*sector;
	t_list		*curr_wall;
	t_wall		*wall;
	int			id;

	id = -1;
	final = ft_sprintf("type:sector\twalls\tneighbors\tg\tl\n");
	curr = editor->sectors;
	while (curr) // we have to remake the ids here so that the neighbor ids will match up;
	{
		((t_sector *)curr->content)->id = ++id;
		curr = curr->next;
	}
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		walls = get_sector_wall_ids(sector);
		temp = ft_sprintf("%d\t%s\t%d\t%d\n", sector->id, walls, sector->gravity, sector->lighting);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		ft_strdel(&walls);
		curr = curr->next;
	}
	return (final);
}

char	*get_sector_wall_slopes(t_sector *sector)
{
	t_list	*curr;
	t_wall	*wall;
	char	*floor;
	char	*ceil;
	char	*final;

	curr = sector->walls;
	floor = NULL;
	ceil = NULL;
	while (curr)
	{
		wall = curr->content;
		if (wall->floor_angle != 0)
			floor = ft_sprintf("%d %d ", wall->id, wall->floor_angle);
		if (wall->ceiling_angle != 0)
			ceil = ft_sprintf("%d %d", wall->id, wall->ceiling_angle);
		curr = curr->next;
	}
	if (!floor)
		floor = ft_sprintf("-1 -1 ");
	if (!ceil)
		ceil = ft_sprintf("-1 -1");
	final = ft_strjoin(floor, ceil);
	ft_strdel(&floor);
	ft_strdel(&ceil);
	return (final);
}

char	*set_fandc(t_editor *editor)
{
	char		*final;
	char		*temp;
	char		*slopes;
	t_list		*curr;
	t_sector	*sector;

	// sl = floor_wall_id floor_angle ceiling_wall_id ceiling_angle;
	final = ft_sprintf("type:f&c\tfh\tch\tftx\tctx\tfs\tcs\tsl\n");
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		slopes = get_sector_wall_slopes(sector);
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%s\n",
				sector->id,
				sector->floor_height, sector->ceiling_height,
				sector->floor_texture, sector->ceiling_texture,
				sector->floor_scale, sector->ceiling_scale,
				slopes);
		ft_strdel(&slopes);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_entity(t_editor *editor)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_entity	*entity;
	int			id;

	id = -1;
	final = ft_sprintf("type:entity\tname\tx\ty\tz\td\n");
	curr = editor->entities;
	while (curr)
	{
		entity = curr->content;
		temp = ft_sprintf("%d\t%s\t%d\t%d\t%d\t%d\n",
				++id, g_entity_data[entity->type].name,
				entity->pos.x, entity->pos.y, entity->z,
				entity->yaw);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_event(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_event	*event;
	int		id;
	int		target_id;

	id = -1;
	final = ft_sprintf("type:event\tTYPE\tACTION\tID\tSECTOR\tMIN\tMAX\tSPEED\n");
	curr = editor->events;
	while (curr)
	{
		event = curr->content;
		event->id = ++id;
		if (event->pointer_type == TYPE_SECTOR)
			target_id = ((t_sector *)event->pointer)->id;
		else
			target_id = ((t_sprite *)event->pointer)->id;
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%s\t%d\t%d\t%d\n",
				event->id, event->type, event->action, target_id,
				event->sector, event->min, event->max, event->speed);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

// Should go through all the points, walls, and sectors and reorder their t.ex ids;
// Remove all lonely points, walls, and sectors, so that we dont have useless stuff that could break the game;
// Remove all events that have sector/sprites that dont exist anymore; or other things in the future;
void	editor_cleanup(t_editor *editor)
{
	(void)editor;
}

void	set_map(t_editor *editor, char *name)
{
	int	fd;

	fd = creat(name, S_IRUSR | S_IWUSR | O_CREAT);
	if (!fd)
	{
		ft_printf("[%s] Couldn\'t open map file : %s\n", __FUNCTION__, name);
		return ;
	}
	editor_cleanup(editor);
	char *delim = ft_sprintf("-\n");
//	ft_printf("Setting Info.\n");
	char *info = set_map_info(editor);
//	ft_printf("Setting Spawn.\n");
	char *spawn = set_spawn(editor);
//	ft_printf("Setting Points.\n");
	char *points = set_points(editor);
//	ft_printf("Setting Walls.\n");
	char *walls = set_walls(editor);
//	ft_printf("Setting Sprites.\n");
	char *sprites = set_sprites(editor);
//	ft_printf("Setting Sectors.\n");
	char *sectors = set_sectors(editor);
//	ft_printf("Setting FandC.\n");
	char *fandc = set_fandc(editor);
//	ft_printf("Setting Entitties.\n");
	char *entity = set_entity(editor);
//	ft_printf("Setting Events.\n");
	char *event = set_event(editor);
	ft_dprintf(fd, "%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
		info, delim,
		spawn, delim,
		points, delim,
		walls, delim,
		sprites, delim,
		sectors, delim,
		fandc, delim,
		entity, delim,
		event, delim);
	ft_strdel(&delim);
	ft_strdel(&info);
	ft_strdel(&points);
	ft_strdel(&walls);
	ft_strdel(&sprites);
	ft_strdel(&sectors);
	ft_strdel(&fandc);
	ft_strdel(&entity);
	ft_strdel(&event);
	close(fd);
	ft_printf("[%s] Map saved succesfully : [%s]\n", __FUNCTION__, name);
}
