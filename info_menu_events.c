/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_menu_events.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:49:12 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:49:12 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	update_amount_info(t_editor *editor)
{
	char	*final;

	final = ft_sprintf("points : %d\nwalls : %d\nsectors : %d\n"
			"entities : %d\nsprites : %d\nevents : %d\n",
			editor->point_amount, editor->wall_amount, editor->sector_amount,
			editor->entity_amount, editor->sprite_amount, editor->event_amount);
	ui_label_set_text(editor->misc_info_label, final);
	ft_strdel(&final);
}

int	update_sector_info(t_editor *editor, t_sector *sector)
{
	char		*final;

	if (sector)
	{
		final = ft_sprintf("iD : %d\npos : %d, %d\nWalls : %d\n",
				sector->id, sector->center.x, sector->center.y,
				sector->wall_amount);
		ui_label_set_text(editor->sector_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sector_info_label, "NONE");
	return (0);
}

int	update_point_info(t_editor *editor, t_point *point)
{
	char	*final;

	if (point)
	{
		final = ft_sprintf("iD : %d\npos : %d, %d\n",
				point->id, point->pos.x, point->pos.y);
		ui_label_set_text(editor->sub_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sub_info_label, "NONE");
	return (0);
}

int	update_sprite_wall_info(t_editor *editor, t_sprite *sprite, t_wall *wall)
{
	char		*final;

	if (sprite)
	{
		sprite = editor->selected_sprite;
		final = ft_sprintf("iD : %d\npos : %d, %d\n",
				sprite->id, sprite->pos.x, sprite->pos.y);
		ui_label_set_text(editor->sprite_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sprite_info_label, "NONE");
	if (wall)
	{
		final = ft_sprintf("iD : %d\np1 : %d, %d\np2 : %d, %d\ndist : %.2f\n",
				wall->id, wall->p1->pos.x, wall->p1->pos.y, wall->p2->pos.x,
				wall->p2->pos.y, distance(wall->p1->pos.v, wall->p2->pos.v, 2));
		ui_label_set_text(editor->sub_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sub_info_label, "NONE");
	return (0);
}

/*
 * NOTE:
 *		Only one of point or walls should be changing the sub info text;
*/
void	info_menu_events(t_editor *editor, SDL_Event e)
{
	char	*final_str;

	if (e.type == SDL_MOUSEMOTION)
	{
		final_str = ft_sprintf("%d, %d",
				editor->mouse_pos.x, editor->mouse_pos.y);
		ui_label_set_text(editor->mouse_info_label, final_str);
		ft_strdel(&final_str);
	}
	update_amount_info(editor);
	update_sector_info(editor, editor->selected_sector);
	if (!update_point_info(editor, editor->selected_point))
		update_sprite_wall_info(editor, editor->selected_sprite,
			editor->selected_wall);
	else
		update_sprite_wall_info(editor, editor->selected_sprite, NULL);
}
