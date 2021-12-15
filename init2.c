/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:29:40 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 14:29:40 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	type_dropdown_init(t_editor *editor)
{
	editor->selection_dropdown_menu
		= ui_layout_get_element(&editor->layout, "type_drop_menu");
	editor->sector_button
		= ui_layout_get_element(&editor->layout, "sector_button");
	editor->entity_button
		= ui_layout_get_element(&editor->layout, "entity_button");
	editor->spawn_button
		= ui_layout_get_element(&editor->layout, "spawn_button");
	editor->event_button
		= ui_layout_get_element(&editor->layout, "event_button");
	ui_dropdown_activate(ui_layout_get_element(
			&editor->layout, "type_dropdown"), editor->sector_button);
}

void	selection_menu_init(t_editor *editor)
{
	editor->menu_toolbox_top
		= ui_layout_get_element(&editor->layout, "menu_toolbox_top");
	editor->menu_selection
		= ui_layout_get_element(&editor->layout, "menu_select_buttons");
	editor->draw_button
		= ui_layout_get_element(&editor->layout, "draw_button");
	editor->select_button
		= ui_layout_get_element(&editor->layout, "select_button");
	((t_ui_radio *)ui_layout_get_element(&editor->layout,
		"select_button_radio")->element)->active = editor->select_button;
	editor->remove_button
		= ui_layout_get_element(&editor->layout, "remove_button");
	editor->save_button
		= ui_layout_get_element(&editor->layout, "save_button");
	editor->edit_button = ui_layout_get_element(&editor->layout, "edit_button");
}

void	sector_skybox_init(t_editor *editor)
{
	editor->sector_skybox_dropdown
		= ui_layout_get_element(&editor->layout, "sector_skybox_dropdown");
	editor->sector_skyboxes[0]
		= ui_layout_get_element(&editor->layout, "sector_skybox_none");
	editor->sector_skyboxes[1]
		= ui_layout_get_element(&editor->layout, "sector_skybox_one");
	editor->sector_skyboxes[2]
		= ui_layout_get_element(&editor->layout, "sector_skybox_two");
	editor->sector_skyboxes[3]
		= ui_layout_get_element(&editor->layout, "sector_skybox_three");
}

void	sector_edit_init2(t_editor *editor)
{
	editor->point_button
		= ui_layout_get_element(&editor->layout, "point_button");
	editor->wall_button
		= ui_layout_get_element(&editor->layout, "wall_button");
	((t_ui_radio *)ui_layout_get_element(&editor->layout,
		"left_button_radio")->element)->active = editor->wall_button;
	editor->close_sector_edit_button
		= ui_layout_get_element(&editor->layout, "close_sector_edit_button");
	editor->sector_edit_ok_button
		= ui_layout_get_element(&editor->layout, "sector_edit_ok_button");
}

void	sector_edit_init(t_editor *editor)
{
	editor->sector_edit_menu
		= ui_layout_get_element(&editor->layout, "sector_edit_menu");
	editor->floor_texture_button
		= ui_layout_get_element(&editor->layout, "floor_texture_button");
	editor->ceiling_texture_button
		= ui_layout_get_element(&editor->layout, "ceiling_texture_button");
	editor->floor_texture_image
		= ui_layout_get_element(&editor->layout, "floor_texture_image");
	editor->ceiling_texture_image
		= ui_layout_get_element(&editor->layout, "ceiling_texture_image");
	editor->floor_height_input
		= ui_layout_get_element(&editor->layout, "floor_height_input");
	editor->ceiling_height_input
		= ui_layout_get_element(&editor->layout, "ceiling_height_input");
	editor->gravity_input
		= ui_layout_get_element(&editor->layout, "gravity_input");
	editor->lighting_input
		= ui_layout_get_element(&editor->layout, "lighting_input");
	editor->floor_texture_scale_input = ui_layout_get_element(
			&editor->layout, "floor_texture_scale_input");
	editor->ceiling_texture_scale_input = ui_layout_get_element(
			&editor->layout, "ceiling_texture_scale_input");
	sector_edit_init2(editor);
	sector_skybox_init(editor);
}
