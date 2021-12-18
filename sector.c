/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:22 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/18 12:47:06 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_sector	*sector_new(void)
{
	t_sector	*sector;

	sector = ft_memalloc(sizeof(t_sector));
	sector->color = random_blue_color();
	sector->floor_height = 0;
	sector->ceiling_height = 20;
	sector->floor_texture = 16;
	sector->ceiling_texture = 2;
	sector->gravity = 20;
	sector->lighting = 0;
	sector->floor_scale = 50.0;
	sector->ceiling_scale = 25.0;
	return (sector);
}

void	sector_free(void *sec, size_t size)
{
	t_sector	*sector;

	sector = sec;
	if (!sector)
		return ;
	ft_lstdel(&sector->walls, &wall_free);
	free(sector);
	(void)size;
}

t_sector	*add_sector(t_editor *editor)
{
	t_sector	*sector;

	sector = sector_new();
	sector->id = get_next_sector_id(editor->sectors);
	++editor->sector_amount;
	add_to_list(&editor->sectors, sector, sizeof(t_sector));
	return (sector);
}

/*
 * Removes all traces of this sector;
*/
int	remove_sector(t_editor *editor, t_sector *sector)
{
	t_list	*curr;
	t_list	*next;

	if (!sector)
		return (0);
	remove_neighbor_from_walls(editor->walls, sector);
	remove_from_list(&editor->sectors, sector);
	curr = sector->walls;
	while (curr)
	{
		next = curr->next;
		remove_wall(editor, curr->content);
		curr = next;
	}
	free(sector);
	sector = NULL;
	ft_printf("[%s] Sector removed from editor->sectors\n", __FUNCTION__);
	--editor->sector_amount;
	return (1);
}

void	sector_render(t_editor *editor, t_sector *sector, Uint32 color)
{
	draw_walls(editor, sector->walls, color);
}
