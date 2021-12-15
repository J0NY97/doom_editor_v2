/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_ui2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:31:08 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:31:08 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	activate_correct_sector_skybox_button(
		t_editor *editor, t_sector *sector)
{
	int	iii;

	iii = -1;
	while (++iii < SKYBOX_AMOUNT)
		if (sector->skybox == -iii)
			ui_dropdown_activate(editor->sector_skybox_dropdown,
				editor->sector_skyboxes[iii]);
}

/*
 * From t_sector update the ui values;
 * happens when you select new sector;
*/
void	set_sector_ui(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	activate_correct_sector_skybox_button(editor, sector);
	sector->floor_texture
		= ft_clamp(sector->floor_texture, 0, MAP_TEXTURE_AMOUNT);
	sector->ceiling_texture
		= ft_clamp(sector->ceiling_texture, 0, MAP_TEXTURE_AMOUNT);
	editor->floor_texture_something.id = sector->floor_texture;
	editor->ceiling_texture_something.id = sector->ceiling_texture;
	ui_input_set_text(editor->floor_height_input,
		ft_b_itoa(sector->floor_height, temp_str));
	ui_input_set_text(editor->ceiling_height_input,
		ft_b_itoa(sector->ceiling_height, temp_str));
	ui_input_set_text(editor->gravity_input,
		ft_b_itoa(sector->gravity, temp_str));
	ui_input_set_text(editor->lighting_input,
		ft_b_itoa(sector->lighting, temp_str));
	ui_input_set_text(editor->floor_texture_scale_input,
		ft_b_ftoa(sector->floor_scale, 2, temp_str));
	ui_input_set_text(editor->ceiling_texture_scale_input,
		ft_b_ftoa(sector->ceiling_scale, 2, temp_str));
	ui_element_image_set(editor->ceiling_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[sector->ceiling_texture]);
	ui_element_image_set(editor->floor_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[sector->floor_texture]);
}
