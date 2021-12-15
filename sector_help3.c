/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_help3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:33:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:33:19 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_sector	*point_inside_which_sector(t_list *sectors, t_vec2i point)
{
	while (sectors)
	{
		if (check_point_in_sector(sectors->content, point) == 1)
			return (sectors->content);
		sectors = sectors->next;
	}
	return (NULL);
}

void	sector_check_errors(t_editor *editor, t_sector *sector)
{
	if (!check_sector_convexity(sector))
	{
		draw_text(editor->drawing_surface, "Not Convex!",
			sector->screen_center, 0xffff0000);
		editor->errors += 1;
	}
	if (sector->ceiling_height - sector->floor_height < 0)
	{
		draw_text(editor->drawing_surface,
			"Floor & Ceiling Height Doesn\'t Make Sense!",
			sector->screen_center, 0xffffff00);
		editor->errors += 1;
	}
}

int	get_sector_id(t_sector *sector)
{
	if (!sector)
		return (-1);
	return (sector->id);
}
