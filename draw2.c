/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:09:17 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/18 12:52:31 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	draw_drawing_wall_len(t_editor *editor)
{
	char	temp_str[20];
	int		len ;
	t_vec2i	middle;

	if (editor->first_point_set)
	{
		len = distance(editor->first_point.v, editor->mouse_pos.v, 2);
		middle = conversion(editor,
				get_middle(editor->first_point, editor->mouse_pos));
		draw_text(editor->drawing_surface, ft_b_itoa(len, temp_str),
			middle, 0xffdeface);
	}
}

void	draw_entities_yaw(t_editor *editor, t_list *entities)
{
	t_entity	*entity;

	while (entities)
	{
		entity = entities->content;
		draw_arrow(editor->drawing_surface,
			conversion(editor, entity->pos), 10, entity->yaw);
		entity->inside_sector
			= point_inside_which_sector(editor->sectors, entity->pos);
		entity_check_errors(editor, entity);
		entities = entities->next;
	}
}

void	draw_entities(t_editor *editor, t_list *entities)
{
	while (entities)
	{
		entity_render(editor, entities->content);
		entities = entities->next;
	}
}

void	draw_sectors(t_editor *editor, t_list *sectors)
{
	t_sector	*sector;
	char		temp[20];
	Uint32		color;

	editor->hovered_sector = NULL;
	while (sectors)
	{
		sector = sectors->content;
		sort_walls(sector->walls);
		sector_render(editor, sector, sector->color);
		sector->center = get_sector_center(sector);
		sector->screen_center = conversion(editor, sector->center);
		color = 0xffffffff;
		if (sector == editor->selected_sector)
			color = 0xff00ff00;
		draw_text(editor->drawing_surface, ft_b_itoa(sector->id, temp),
			sector->screen_center, color);
		sector_check_errors(editor, sector);
		if (vec2_in_vec4(editor->win_main->mouse_pos,
				vec4i(sector->screen_center.x - 10,
					sector->screen_center.y - 10, 20, 20)))
			editor->hovered_sector = sector;
		sectors = sectors->next;
	}
}
