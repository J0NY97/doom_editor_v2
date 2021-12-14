/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:47:54 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:47:54 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	spawn_events(t_editor *editor)
{
	char	temp_str[20];

	if (editor->spawn_button->state == UI_STATE_CLICK)
		editor->spawn_edit_menu->show = 1;
	else
		editor->spawn_edit_menu->show = 0;
	if (!editor->spawn_edit_menu->show)
		return ;
	if (editor->spawn_button->was_click)
		ui_input_set_text(editor->spawn_yaw_input,
			ft_b_itoa(editor->spawn.yaw, temp_str));
	if (editor->draw_button->state == UI_STATE_CLICK)
		if (!hover_over_open_menus(editor))
			if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
				editor->spawn.pos = editor->mouse_pos;
	if (editor->select_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		editor->spawn.pos = vec2i_add(editor->spawn.pos, editor->move_amount);
	if (ui_input_exit(editor->spawn_yaw_input))
	{
		editor->spawn.yaw = ft_clamp(
				ft_atoi(ui_input_get_text(editor->spawn_yaw_input)), 0, 360);
		ui_input_set_text(editor->spawn_yaw_input,
			ft_b_itoa(editor->spawn.yaw, temp_str));
	}
}