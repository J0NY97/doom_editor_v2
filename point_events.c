/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   point_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:43:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:43:19 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	point_select_events(t_editor *editor)
{
	t_point	*point;

	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
	{
		point = get_point_from_sector_around_radius(
				editor->selected_sector, editor->mouse_pos, 1.0f);
		if (point)
			editor->selected_point = point;
	}
	else if (editor->selected_point
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		editor->selected_point->pos
			= vec2i_add(editor->selected_point->pos, editor->move_amount);
}

void	point_events(t_editor *editor)
{
	if (editor->sector_button->state == UI_STATE_CLICK
		&& editor->point_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_sector)
		{
			if (editor->point_button->state == UI_STATE_CLICK)
				point_select_events(editor);
			else
				editor->selected_point = NULL;
		}
		else
			editor->selected_point = NULL;
	}
	else
		editor->selected_point = NULL;
}
