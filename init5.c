/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init5.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:34:00 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 14:34:00 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	event_type_init(t_editor *editor)
{
	editor->event_type_dropdown
		= ui_layout_get_element(&editor->layout, "event_type_dropdown");
	editor->event_type_floor
		= ui_layout_get_element(&editor->layout, "event_type_floor");
	editor->event_type_ceiling
		= ui_layout_get_element(&editor->layout, "event_type_ceiling");
	editor->event_type_light
		= ui_layout_get_element(&editor->layout, "event_type_light");
	editor->event_type_store
		= ui_layout_get_element(&editor->layout, "event_type_store");
	editor->event_type_hazard
		= ui_layout_get_element(&editor->layout, "event_type_hazard");
	editor->event_type_audio
		= ui_layout_get_element(&editor->layout, "event_type_audio");
	editor->event_type_spawn
		= ui_layout_get_element(&editor->layout, "event_type_spawn");
}

void	event_action_init(t_editor *editor)
{
	editor->event_action_dropdown
		= ui_layout_get_element(&editor->layout, "event_action_dropdown");
	editor->event_action_click_button
		= ui_layout_get_element(&editor->layout, "event_action_click");
	editor->event_action_shoot_button
		= ui_layout_get_element(&editor->layout, "event_action_shoot");
	editor->event_action_sector_button
		= ui_layout_get_element(&editor->layout, "event_action_sector");
	editor->event_action_null_button
		= ui_layout_get_element(&editor->layout, "event_action_null");
}

void	event_menu_init(t_editor *editor)
{
	editor->event_scrollbar
		= ui_layout_get_element(&editor->layout, "event_scrollbar");
	editor->event_edit_menu
		= ui_layout_get_element(&editor->layout, "event_edit_menu");
	editor->add_event_button
		= ui_layout_get_element(&editor->layout, "add_event_button");
	editor->remove_event_button
		= ui_layout_get_element(&editor->layout, "remove_event_button");
	editor->event_id_dropdown
		= ui_layout_get_element(&editor->layout, "event_id_dropdown");
	editor->event_id_menu
		= ui_dropdown_get_menu_element(editor->event_id_dropdown);
	editor->event_sector_input
		= ui_layout_get_element(&editor->layout, "event_sector_input");
	editor->event_min_input
		= ui_layout_get_element(&editor->layout, "event_min_input");
	editor->event_max_input
		= ui_layout_get_element(&editor->layout, "event_max_input");
	editor->event_speed_input
		= ui_layout_get_element(&editor->layout, "event_speed_input");
	editor->event_menu
		= ui_layout_get_element(&editor->layout, "event_menu");
	event_type_init(editor);
	event_action_init(editor);
}
