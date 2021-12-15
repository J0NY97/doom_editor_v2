/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:26 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 11:42:19 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*set_map_info(t_editor *editor)
{
	char	*final;
	char	*map_type;

	if (editor->map_type == MAP_TYPE_STORY)
		map_type = ft_strdup("story");
	else
		map_type = ft_strdup("endless");
	final = ft_sprintf("type:map\ttype\tscale\tvert\twall\tsect\n"
			"%d\t%s\t%.2f\t%d\t%d\t%d\n",
			0,
			map_type,
			editor->map_scale,
			ft_lstlen(editor->points),
			ft_lstlen(editor->walls),
			ft_lstlen(editor->sectors));
	ft_strdel(&map_type);
	return (final);
}

char	*set_spawn(t_editor *editor)
{
	char	*final;
	int		inside_sector_id;

	inside_sector_id = -1;
	if (editor->spawn.inside_sector)
		inside_sector_id = editor->spawn.inside_sector->id;
	final = ft_sprintf("type:spawn\tx\ty\tz\tyaw\tsector\n"
			"%d\t%d\t%d\t%d\t%d\t%d\n",
			0,
			editor->spawn.pos.x,
			editor->spawn.pos.y,
			editor->spawn.z,
			editor->spawn.yaw,
			inside_sector_id);
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
	final = ft_sprintf("type:entity\tname\tx\ty\tz\td\tsector\n");
	curr = editor->entities;
	while (curr)
	{
		entity = curr->content;
		temp = ft_sprintf("%d\t%s\t%d\t%d\t%d\t%d\t%d\n",
				++id, g_entity_data[entity->type - 1].name,
				entity->pos.x, entity->pos.y, entity->z,
				entity->yaw, get_sector_id(entity->inside_sector));
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

void	first_half(t_editor *editor, int fd, char *delim)
{
	char	*temp;

	temp = set_map_info(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_spawn(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_points(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_walls(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_sprites(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_sectors(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_fandc(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
}

void	set_map(t_editor *editor, char *name)
{
	int		fd;
	char	*temp;
	char	delim[3];

	fd = creat(name, S_IRUSR | S_IWUSR | O_CREAT);
	if (!fd)
	{
		ft_printf("[%s] Couldn\'t open map file : %s\n", __FUNCTION__, name);
		return ;
	}
	ft_strcpy(delim, "-\n");
	first_half(editor, fd, delim);
	temp = set_entity(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	temp = set_event(editor);
	ft_dprintf(fd, "%s%s", temp, delim);
	ft_strdel(&temp);
	close(fd);
	ft_printf("[%s] Map saved succesfully : [%s]\n", __FUNCTION__, name);
}
