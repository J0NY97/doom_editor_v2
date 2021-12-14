/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:26:42 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:26:42 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	set_entity_ui(t_editor *editor, t_entity *entity)
{
	char	temp_str[20];

	ui_input_set_text(editor->entity_yaw_input,
		ft_b_itoa(entity->yaw, temp_str));
	ui_slider_value_set(editor->entity_yaw_slider, entity->yaw);
	ui_input_set_text(editor->entity_z_input, ft_b_itoa(entity->z, temp_str));
	ui_element_image_set(editor->entity_image, UI_STATE_DEFAULT,
		editor->entity_texture_surfaces[entity->type]);
	ui_dropdown_activate(editor->entity_dropdown,
		ui_dropdown_get_button_with_text(editor->entity_dropdown,
			g_entity_data[entity->type - 1].name));
}

void	get_entity_ui(t_editor *editor, t_entity *entity)
{
	char	temp_str[20];

	if (ui_input_exit(editor->entity_yaw_input))
		entity->yaw = ft_clamp(ft_atoi(
					ui_input_get_text(editor->entity_yaw_input)), 0, 360);
	else if (ui_slider_updated(editor->entity_yaw_slider))
		entity->yaw = ui_slider_value_get(editor->entity_yaw_slider);
	if (ui_input_exit(editor->entity_yaw_input))
		ui_slider_value_set(editor->entity_yaw_slider, entity->yaw);
	if (ui_slider_updated(editor->entity_yaw_slider))
		ui_input_set_text(editor->entity_yaw_input,
			ft_b_itoa(entity->yaw, temp_str));
	if (ui_input_exit(editor->entity_z_input))
		entity->z = ft_atoi(ui_input_get_text(editor->entity_z_input));
	if (ui_dropdown_exit(editor->entity_dropdown)
		&& ui_dropdown_active(editor->entity_dropdown))
	{
		entity->type = get_entity_type(
				ui_dropdown_active_text(editor->entity_dropdown));
		ui_element_image_set(editor->entity_image, UI_STATE_DEFAULT,
			editor->entity_texture_surfaces[entity->type]);
	}
}

void	entity_check_errors(t_editor *editor, t_entity *entity)
{
	if (!entity->inside_sector)
	{
		draw_text(editor->drawing_surface, "Not Inside Sector!",
			conversion(editor, entity->pos), 0xffff0000);
		editor->errors += 1;
	}
	else if (entity->inside_sector
		&& (entity->z < entity->inside_sector->floor_height
			|| entity->z > entity->inside_sector->ceiling_height))
	{
		draw_text(editor->drawing_surface, "Z not between Floor & Ceiling!",
			conversion(editor, entity->pos), 0xffff0000);
		editor->errors += 1;
	}
}
