/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map_sector.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:39:51 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:39:51 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*create_wall_ids(t_list *walls)
{
	char	*final;
	char	temp_str[20];
	t_wall	*wall;

	final = NULL;
	while (walls)
	{
		wall = walls->content;
		ft_stradd(&final, ft_b_itoa(wall->id, temp_str));
		ft_straddchar(&final, ' ');
		walls = walls->next;
	}
	return (final);
}

char	*create_neighbor_ids(t_list *walls)
{
	char	*final;
	char	temp_str[20];
	t_wall	*wall;

	final = NULL;
	while (walls)
	{
		wall = walls->content;
		if (wall->neighbor)
			ft_stradd(&final, ft_b_itoa(wall->neighbor->id, temp_str));
		else
			ft_stradd(&final, "-1");
		ft_straddchar(&final, ' ');
		walls = walls->next;
	}
	return (final);
}

char	*get_sector_wall_ids(t_sector *sector)
{
	char	*final;
	char	*walls;
	char	*neighbors;

	walls = create_wall_ids(sector->walls);
	neighbors = create_neighbor_ids(sector->walls);
	final = ft_strdup(walls);
	ft_straddchar(&final, '\t');
	ft_stradd(&final, neighbors);
	ft_strdel(&walls);
	ft_strdel(&neighbors);
	return (final);
}

char	*create_sector_string(t_sector *sector)
{
	char	*final;
	char	*walls;

	walls = get_sector_wall_ids(sector);
	final = ft_sprintf("%d\t%s\t%d\t%d\n",
			sector->id, walls, sector->gravity, sector->lighting);
	ft_strdel(&walls);
	return (final);
}

char	*set_sectors(t_editor *editor)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_sector	*sector;

	final = ft_sprintf("type:sector\twalls\tneighbors\tg\tl\n");
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		temp = create_sector_string(sector);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}
