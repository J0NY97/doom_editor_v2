/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_window_events.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:50:41 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:50:41 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	save_button_events(t_editor *editor)
{
	char	*actual_full_path;

	if (ft_strlen(ui_input_get_text(editor->name_input)) < 1)
		ft_printf("[%s] Map name len is less than 1.\n", __FUNCTION__);
	else
	{
		if (editor->map_type == MAP_TYPE_STORY)
			actual_full_path = ft_strjoiner(MAP_PATH,
					ui_input_get_text(editor->name_input), ".dnds", NULL);
		else
			actual_full_path = ft_strjoiner(MAP_PATH,
					ui_input_get_text(editor->name_input), ".dnde", NULL);
		set_map(editor, actual_full_path);
		ft_strdel(&actual_full_path);
		ui_window_flag_set(editor->win_save, UI_WINDOW_HIDE);
		ui_window_flag_set(editor->win_main, UI_WINDOW_RAISE);
		send_info_message(editor, "Map Saved Successfully!");
	}
}

void	save_window_events(t_editor *editor)
{
	if (editor->save_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_save, UI_WINDOW_SHOW | UI_WINDOW_RAISE);
	if (!editor->win_save->show)
		return ;
	if (editor->endless_checkbox->was_click)
		editor->map_type = MAP_TYPE_ENDLESS;
	else if (editor->story_checkbox->was_click)
		editor->map_type = MAP_TYPE_STORY;
	ui_checkbox_toggle_accordingly(editor->endless_checkbox,
		editor->map_type == MAP_TYPE_ENDLESS);
	ui_checkbox_toggle_accordingly(editor->story_checkbox,
		editor->map_type == MAP_TYPE_STORY);
	if (ui_button(editor->confirm_save_button))
		save_button_events(editor);
	if (editor->win_save->wants_to_close)
		ui_window_flag_set(editor->win_main, UI_WINDOW_RAISE);
}

void	edit_window_events(t_editor *editor)
{
	if (editor->edit_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_edit, UI_WINDOW_SHOW | UI_WINDOW_RAISE);
	if (!editor->win_edit->show)
		return ;
	if (ui_input_exit(editor->map_scale_input))
		editor->map_scale = ft_atof(ui_input_get_text(editor->map_scale_input));
}
