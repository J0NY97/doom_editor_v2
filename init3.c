/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:30:52 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 14:30:52 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	wall_skybox_init(t_editor *editor)
{
	editor->wall_skybox_dropdown
		= ui_layout_get_element(&editor->layout, "wall_skybox_dropdown");
	editor->wall_skyboxes[0]
		= ui_layout_get_element(&editor->layout, "wall_skybox_none");
	editor->wall_skyboxes[1]
		= ui_layout_get_element(&editor->layout, "wall_skybox_one");
	editor->wall_skyboxes[2]
		= ui_layout_get_element(&editor->layout, "wall_skybox_two");
	editor->wall_skyboxes[3]
		= ui_layout_get_element(&editor->layout, "wall_skybox_three");
}

void	wall_edit_init(t_editor *editor)
{
	editor->menu_wall_edit
		= ui_layout_get_element(&editor->layout, "menu_wall_edit");
	editor->close_wall_edit_button
		= ui_layout_get_element(&editor->layout, "close_wall_edit_button");
	editor->solid_checkbox
		= ui_layout_get_element(&editor->layout, "solidity_checkbox");
	editor->portal_checkbox
		= ui_layout_get_element(&editor->layout, "portal_checkbox");
	editor->split_wall_button
		= ui_layout_get_element(&editor->layout, "split_wall_button");
	editor->wall_texture_button
		= ui_layout_get_element(&editor->layout, "wall_texture_button");
	editor->wall_texture_image
		= ui_layout_get_element(&editor->layout, "wall_texture_image");
	editor->portal_texture_button
		= ui_layout_get_element(&editor->layout, "portal_texture_button");
	editor->portal_texture_image
		= ui_layout_get_element(&editor->layout, "portal_texture_image");
	editor->floor_wall_angle_input
		= ui_layout_get_element(&editor->layout, "floor_wall_angle_input");
	editor->ceiling_wall_angle_input = ui_layout_get_element(
			&editor->layout, "ceiling_wall_angle_input");
	editor->wall_texture_scale_input = ui_layout_get_element(
			&editor->layout, "wall_texture_scale_input");
	wall_skybox_init(editor);
}

void	wall_render_init(t_editor *editor)
{
	editor->wall_render
		= ui_layout_get_element(&editor->layout, "wall_render");
}

void	sprite_edit_init(t_editor *editor)
{
	editor->sprite_edit_menu
		= ui_layout_get_element(&editor->layout, "sprite_edit_menu");
	editor->sprite_add_button
		= ui_layout_get_element(&editor->layout, "sprite_add_button");
	editor->sprite_remove_button
		= ui_layout_get_element(&editor->layout, "sprite_remove_button");
	editor->sprite_scale_input
		= ui_layout_get_element(&editor->layout, "sprite_scale_input");
	editor->sprite_type_dropdown
		= ui_layout_get_element(&editor->layout, "sprite_type_dropdown");
	editor->sprite_type_static
		= ui_layout_get_element(&editor->layout, "sprite_type_static");
	editor->sprite_type_loop
		= ui_layout_get_element(&editor->layout, "sprite_type_loop");
	editor->sprite_type_action
		= ui_layout_get_element(&editor->layout, "sprite_type_action");
	editor->sprite_texture_button
		= ui_layout_get_element(&editor->layout, "sprite_texture_button");
	editor->sprite_texture_image
		= ui_layout_get_element(&editor->layout, "sprite_texture_image");
	editor->sprite_x_input
		= ui_layout_get_element(&editor->layout, "sprite_x_input");
	editor->sprite_y_input
		= ui_layout_get_element(&editor->layout, "sprite_y_input");
	wall_render_init(editor);
}
