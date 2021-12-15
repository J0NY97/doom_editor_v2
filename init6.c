/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init6.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:38:50 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 14:38:50 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	info_init(t_editor *editor)
{
	editor->sector_info_label
		= ui_layout_get_element(&editor->layout, "selected_sector_info");
	editor->mouse_info_label
		= ui_layout_get_element(&editor->layout, "mouse_hover_info");
	editor->sub_info_label
		= ui_layout_get_element(&editor->layout, "selected_sub_info");
	editor->sprite_info_label
		= ui_layout_get_element(&editor->layout, "selected_sprite_info");
	editor->misc_info_label
		= ui_layout_get_element(&editor->layout, "misc_info");
	editor->info_label
		= ui_layout_get_element(&editor->layout, "info_label");
	editor->sector_hover_info_menu
		= ui_layout_get_element(&editor->layout, "sector_hover_info_menu");
	editor->sector_hover_info_label
		= ui_layout_get_element(&editor->layout, "sector_hover_info_label");
}

void	save_window_init(t_editor *editor)
{
	editor->win_save
		= ui_layout_get_window(&editor->layout, "win_save");
	editor->endless_checkbox
		= ui_layout_get_element(&editor->layout, "endless_checkbox");
	editor->story_checkbox
		= ui_layout_get_element(&editor->layout, "story_checkbox");
	editor->name_input
		= ui_layout_get_element(&editor->layout, "name_input");
	editor->confirm_save_button
		= ui_layout_get_element(&editor->layout, "confirm_save_button");
}

void	edit_window_init(t_editor *editor)
{
	editor->win_edit
		= ui_layout_get_window(&editor->layout, "win_edit");
	editor->map_scale_input
		= ui_layout_get_element(&editor->layout, "map_scale_input");
	editor->map_scale = 1.0f;
}
