/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:03:58 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/12 10:16:08 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Only the map type is usefull here, for the correct tickbox to be ticked;
*/
void	get_map_info(t_editor *editor, char **args)
{
	ft_printf("[%s]\n", __FUNCTION__);
	editor->map_type = MAP_TYPE_ENDLESS;
	if (ft_strnequ(args[1], "story", 5))
		editor->map_type = MAP_TYPE_STORY;
}

void	get_spawn(t_editor *editor, char **lines, int *i)
{
	char	**args;

	ft_printf("[%s]\n", __FUNCTION__);
	args = ft_strsplit(lines[*i + 1], '\t');
	editor->spawn.pos.x = ft_atoi(args[1]);
	editor->spawn.pos.y = ft_atoi(args[2]);
	editor->spawn.z = ft_atoi(args[3]);
	editor->spawn.yaw = ft_atoi(args[4]);
	*i += 2;
	ft_arraydel(args);
}

void	helper_pelper(
		t_editor *editor, char **lines, int *i, void (*f)(t_editor *, char **))
{
	char		**args;

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

void	decision_maker(t_editor *editor, char *type, char **lines, int *i)
{
	if (ft_strnequ(type, "type:map", 8))
		helper_pelper(editor, lines, i, &get_map_info);
	else if (ft_strnequ(type, "type:spawn", 10))
		get_spawn(editor, lines, i);
	else if (ft_strnequ(type, "type:point", 10))
		get_points(editor, lines, i);
	else if (ft_strnequ(type, "type:wall", 9))
		get_walls(editor, lines, i);
	else if (ft_strnequ(type, "type:wsprite", 12))
		helper_pelper(editor, lines, i, &get_sprites);
	else if (ft_strnequ(type, "type:sector", 11))
		get_sectors(editor, lines, i);
	else if (ft_strnequ(type, "type:f&c", 8))
		helper_pelper(editor, lines, i, &get_fc);
	else if (ft_strnequ(type, "type:entity", 11))
		get_entity(editor, lines, i);
	else if (ft_strnequ(type, "type:event", 10))
		helper_pelper(editor, lines, i, &get_events);
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
	while (lines && lines[++i])
		decision_maker(editor, lines[i], lines, &i);
	ft_arraydel(lines);
	ft_strdel(&file_content);
	ft_printf("[%s] Success getting map.\n", __FUNCTION__);
}
