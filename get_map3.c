/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:09:29 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 15:09:29 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * search from 'list' walls with 'id' in 'id_str' (delim with ' '(space))
 * 	and add to 'sector'->walls;
 * and also settings neighbor ids at the same time, becuase why not;
*/
void	get_sector_walls(
		t_list *list, char *id_str, char *neighbor_str, t_sector *sector)
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
			ft_printf("[%s] No wall with id : %d.\n", __FUNCTION__, id);
	}
	ft_arraydel(wall_ids);
	ft_arraydel(neigh_ids);
}

/*
 * From the sector wall goes through all
 * 		and gets the actual sector from the neighbor id in the walls;
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
			((t_wall *)curr_wall->content)->neighbor
				= get_sector_by_id_from_list(editor->sectors,
					((t_wall *)curr_wall->content)->neighbor_id);
			curr_wall = curr_wall->next;
		}
		curr_sec = curr_sec->next;
	}
}

void	get_sectors(t_editor *editor, char **lines, int *i)
{
	char		**args;
	t_sector	*sector;

	ft_printf("[%s]\n", __FUNCTION__);
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
}

void	set_slopes(t_list *walls, char **slopes)
{
	t_wall	*wall;

	if (ft_atoi(slopes[0]) != -1)
	{
		wall = get_wall_with_id(walls, ft_atoi(slopes[0]));
		if (wall)
			wall->floor_angle = ft_atoi(slopes[1]);
	}
	if (ft_atoi(slopes[2]) != -1)
	{
		wall = get_wall_with_id(walls, ft_atoi(slopes[2]));
		if (wall)
			wall->ceiling_angle = ft_atoi(slopes[3]);
	}
}

/*
 * NOTE:
 *		Returns if we cant find sector with correct id;
*/
void	get_fc(t_editor *editor, char **args)
{
	char		**slopes;
	t_sector	*sector;
	int			id;

	ft_printf("[%s]\n", __FUNCTION__);
	id = ft_atoi(args[0]);
	sector = get_sector_by_id_from_list(editor->sectors, id);
	if (!sector)
		return ;
	sector->floor_height = ft_atoi(args[1]);
	sector->ceiling_height = ft_atoi(args[2]);
	sector->floor_texture = ft_atoi(args[3]);
	sector->ceiling_texture = ft_atoi(args[4]);
	if (sector->floor_texture < 0)
		sector->skybox = sector->floor_texture;
	sector->floor_scale = ft_atof(args[5]);
	sector->ceiling_scale = ft_atof(args[6]);
	slopes = ft_strsplit(args[7], ' ');
	set_slopes(sector->walls, slopes);
	ft_arraydel(slopes);
}
