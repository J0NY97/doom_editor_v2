/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_ui.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:48:18 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:48:18 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * When you select a sprite, update ui;
*/
void	set_sprite_ui(t_editor *editor, t_sprite *sprite)
{
	char	temp_str[20];

	if (!sprite || !editor)
		return ;
	editor->sprite_texture_something.id = sprite->texture_id;
	ui_element_image_set(editor->sprite_texture_image, UI_STATE_AMOUNT,
		editor->wall_textures[sprite->texture_id]);
	ui_input_set_text(editor->sprite_scale_input,
		ft_b_ftoa(sprite->scale, 2, temp_str));
	if (sprite->type == STATIC)
		ui_dropdown_activate(editor->sprite_type_dropdown,
			editor->sprite_type_static);
	else if (sprite->type == LOOP)
		ui_dropdown_activate(editor->sprite_type_dropdown,
			editor->sprite_type_loop);
	else if (sprite->type == ACTION)
		ui_dropdown_activate(editor->sprite_type_dropdown,
			editor->sprite_type_action);
	ui_input_set_text(editor->sprite_x_input,
		ft_b_itoa(sprite->pos.x, temp_str));
	ui_input_set_text(editor->sprite_y_input,
		ft_b_itoa(sprite->pos.y, temp_str));
}

void	get_sprite_ui(t_editor *editor, t_sprite *sprite)
{
	if (!sprite || !editor)
		return ;
	sprite->texture_id = editor->sprite_texture_something.id;
	if (ui_input_exit(editor->sprite_x_input))
		sprite->pos.x = ft_atoi(ui_input_get_text(editor->sprite_x_input));
	if (ui_input_exit(editor->sprite_y_input))
		sprite->pos.y = ft_atoi(ui_input_get_text(editor->sprite_y_input));
	if (ui_input_exit(editor->sprite_scale_input))
		sprite->scale = ft_atof(ui_input_get_text(editor->sprite_scale_input));
	if (ui_dropdown_exit(editor->sprite_type_dropdown))
	{
		if (editor->sprite_type_loop->state == UI_STATE_CLICK)
			sprite->type = LOOP;
		else if (editor->sprite_type_action->state == UI_STATE_CLICK)
			sprite->type = ACTION;
		else
			sprite->type = STATIC;
	}
}
