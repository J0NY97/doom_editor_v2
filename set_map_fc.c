/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map_fc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:40:53 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:40:53 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*get_wall_slopes(t_list *walls)
{
	t_wall	*wall;
	char	*floor;
	char	*ceil;
	char	*final;

	floor = NULL;
	ceil = NULL;
	while (walls)
	{
		wall = walls->content;
		if (wall->floor_angle != 0)
			floor = ft_sprintf("%d %d ", wall->id, wall->floor_angle);
		if (wall->ceiling_angle != 0)
			ceil = ft_sprintf("%d %d", wall->id, wall->ceiling_angle);
		walls = walls->next;
	}
	if (!floor)
		floor = ft_sprintf("-1 0 ");
	if (!ceil)
		ceil = ft_sprintf("-1 0");
	final = ft_strjoin(floor, ceil);
	ft_strdel(&floor);
	ft_strdel(&ceil);
	return (final);
}

int	get_floor_texture(t_sector *sector)
{
	if (sector->skybox != 0)
		return (sector->skybox);
	return (sector->floor_texture);
}

int	get_ceiling_texture(t_sector *sector)
{
	if (sector->skybox != 0)
		return (sector->skybox);
	return (sector->ceiling_texture);
}

char	*set_fandc(t_editor *editor)
{
	char		*final;
	char		*temp;
	char		*slopes;
	t_list		*curr;
	t_sector	*sector;

	final = ft_sprintf("type:f&c\tfh\tch\tftx\tctx\tfs\tcs\tsl\n");
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		slopes = get_wall_slopes(sector->walls);
		temp = ft_sprintf("%d\t%d\t%d\t%d\t%d\t%.2f\t%.2f\t%s\n",
				sector->id,
				sector->floor_height, sector->ceiling_height,
				get_floor_texture(sector), get_ceiling_texture(sector),
				sector->floor_scale, sector->ceiling_scale,
				slopes);
		ft_strdel(&slopes);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}
