/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_ui.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:30:45 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:30:45 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	get_height_inputs(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	if (ui_input_exit(editor->floor_height_input))
	{
		sector->floor_height = ft_min(ft_atoi(ui_input_get_text(
						editor->floor_height_input)), sector->ceiling_height);
		ft_b_itoa(sector->floor_height, temp_str);
		ui_input_set_text(editor->floor_height_input, temp_str);
	}
	if (ui_input_exit(editor->ceiling_height_input))
	{
		sector->ceiling_height = ft_max(ft_atoi(ui_input_get_text(
						editor->ceiling_height_input)), sector->floor_height);
		ft_b_itoa(sector->ceiling_height, temp_str);
		ui_input_set_text(editor->ceiling_height_input, temp_str);
	}
}

/*
 * Not grenade launcher,
 * 	but gravity and lighting;
*/
void	get_gl_inputs(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	if (ui_input_exit(editor->gravity_input))
	{
		sector->gravity = ft_clamp(ft_atoi(ui_input_get_text(
						editor->gravity_input)), 0, 100);
		ft_b_itoa(sector->gravity, temp_str);
		ui_input_set_text(editor->gravity_input, temp_str);
	}
	if (ui_input_exit(editor->lighting_input))
	{
		sector->lighting = ft_clamp(ft_atoi(ui_input_get_text(
						editor->lighting_input)), -250, 255);
		ft_b_itoa(sector->lighting, temp_str);
		ui_input_set_text(editor->lighting_input, temp_str);
	}
}

void	get_texture_scale_inputs(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	if (ui_input_exit(editor->floor_texture_scale_input))
	{
		sector->floor_scale = ft_fclamp(ft_atof(ui_input_get_text(
						editor->floor_texture_scale_input)), 0.1f, 100.0f);
		ft_b_ftoa(sector->floor_scale, 2, temp_str);
		ui_input_set_text(editor->floor_texture_scale_input, temp_str);
	}
	if (ui_input_exit(editor->ceiling_texture_scale_input))
	{
		sector->ceiling_scale = ft_fclamp(ft_atof(ui_input_get_text(
						editor->ceiling_texture_scale_input)), 0.1f, 100.0f);
		ft_b_ftoa(sector->ceiling_scale, 2, temp_str);
		ui_input_set_text(editor->ceiling_texture_scale_input, temp_str);
	}
}

/*
 * From ui update the t_sector values;
 * when you edit sector;
 *
 * TODO: As short as it can be, at some point split this into multiple
 * 		functions ( not yet, convolution otherwise );
*/
void	get_sector_ui(t_editor *editor, t_sector *sector)
{
	t_ui_element	*skybox_active;
	int				iii;

	sector->floor_texture = editor->floor_texture_something.id;
	sector->ceiling_texture = editor->ceiling_texture_something.id;
	if (ui_dropdown_exit(editor->sector_skybox_dropdown))
	{
		iii = -1;
		skybox_active = ui_dropdown_active(editor->sector_skybox_dropdown);
		while (++iii < SKYBOX_AMOUNT)
			if (skybox_active == editor->sector_skyboxes[iii])
				sector->skybox = -iii;
	}
	get_height_inputs(editor, sector);
	get_gl_inputs(editor, sector);
	get_texture_scale_inputs(editor, sector);
}
