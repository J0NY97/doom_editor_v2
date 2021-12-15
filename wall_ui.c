/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 12:28:17 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 12:28:17 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * When you select new wall update ui;
*/
void	set_wall_ui(t_editor *editor, t_wall *wall)
{
	char	temp_str[20];
	int		iii;

	iii = SKYBOX_AMOUNT;
	while (--iii >= 0)
		if (wall->skybox == -iii)
			ui_dropdown_activate(editor->wall_skybox_dropdown,
				editor->wall_skyboxes[iii]);
	editor->wall_texture_something.id = wall->wall_texture;
	editor->portal_texture_something.id = wall->portal_texture;
	ui_element_image_set(editor->wall_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[wall->wall_texture]);
	ui_element_image_set(editor->portal_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[wall->portal_texture]);
	ui_checkbox_toggle_accordingly(editor->solid_checkbox, wall->solid);
	ui_checkbox_toggle_accordingly(editor->portal_checkbox, wall->neighbor);
	ui_input_set_text(editor->floor_wall_angle_input,
		ft_b_itoa(wall->floor_angle, temp_str));
	ui_input_set_text(editor->ceiling_wall_angle_input,
		ft_b_itoa(wall->ceiling_angle, temp_str));
	ui_input_set_text(editor->wall_texture_scale_input,
		ft_b_ftoa(wall->texture_scale, 2, temp_str));
}

/*
 * Update ui when you edit wall;
 *
 * TODO : split this function into multiple functions ( adds convolution imo)
*/
void	get_wall_ui2(t_editor *editor, t_wall *wall)
{
	int				angle;
	char			temp_str[20];

	if (ui_input_exit(editor->floor_wall_angle_input))
	{
		angle = ft_clamp(ft_atoi(ui_input_get_text(
						editor->floor_wall_angle_input)), -45, 45);
		if (angle != 0)
			remove_wall_list_angles(wall->parent_sector->walls, 0);
		wall->floor_angle = angle;
		ft_b_itoa(wall->floor_angle, temp_str);
		ui_input_set_text(editor->floor_wall_angle_input, temp_str);
	}
	if (ui_input_exit(editor->ceiling_wall_angle_input))
	{
		angle = ft_clamp(ft_atoi(ui_input_get_text(
						editor->ceiling_wall_angle_input)), -45, 45);
		if (angle != 0)
			remove_wall_list_angles(wall->parent_sector->walls, 1);
		wall->ceiling_angle = angle;
		ft_b_itoa(wall->ceiling_angle, temp_str);
		ui_input_set_text(editor->ceiling_wall_angle_input, temp_str);
	}
}

void	get_wall_ui3(t_editor *editor, t_wall *wall)
{
	char			temp_str[20];

	if (ui_input_exit(editor->wall_texture_scale_input))
	{
		wall->texture_scale = ft_fclamp(ft_atof(ui_input_get_text(
						editor->wall_texture_scale_input)), 0.1f, 100.0f);
		ft_b_ftoa(wall->texture_scale, 2, temp_str);
		ui_input_set_text(editor->wall_texture_scale_input, temp_str);
	}
}

void	get_wall_ui(t_editor *editor, t_wall *wall)
{
	t_ui_element	*skybox_active;
	int				iii;

	if (ui_dropdown_exit(editor->wall_skybox_dropdown))
	{
		skybox_active = ui_dropdown_active(editor->wall_skybox_dropdown);
		iii = -1;
		while (++iii < SKYBOX_AMOUNT)
			if (skybox_active == editor->wall_skyboxes[iii])
				wall->skybox = -iii;
	}
	wall->wall_texture = editor->wall_texture_something.id;
	wall->portal_texture = editor->portal_texture_something.id;
	if (!editor->portal_checkbox->is_toggle)
		wall->neighbor = NULL;
	if (editor->portal_checkbox->is_toggle)
		if (!can_you_make_portal_of_this_wall(editor->sectors,
				wall->parent_sector, wall))
			ui_checkbox_toggle_off(editor->portal_checkbox);
	if (!wall->neighbor)
		ui_checkbox_toggle_on(editor->solid_checkbox);
	wall->solid = editor->solid_checkbox->is_toggle;
	get_wall_ui2(editor, wall);
	get_wall_ui3(editor, wall);
}
