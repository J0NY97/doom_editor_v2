/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:53:31 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/18 12:46:47 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	select_sprite(t_editor *editor)
{
	t_vec2i		actual_mouse_pos;
	t_sprite	*sprite;

	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& ui_element_is_hover(editor->wall_render))
	{
		veci_sub(actual_mouse_pos.v, editor->win_main->mouse_pos.v,
			editor->wall_render->screen_pos.v, 2);
		sprite = get_sprite_from_list_at_pos(editor->selected_wall->sprites,
				actual_mouse_pos);
		if (sprite)
		{
			editor->selected_sprite = sprite;
			set_sprite_ui(editor, editor->selected_sprite);
		}
	}
}

void	edit_sprite(t_editor *editor)
{
	t_vec2	mouse_diff;
	char	temp_str[20];

	if (!editor->selected_sprite)
		return ;
	if (editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& ui_element_is_hover(editor->sprite_edit_menu))
	{
		mouse_diff.x = editor->win_main->mouse_pos.x
			- editor->win_main->mouse_pos_prev.x;
		mouse_diff.y = editor->win_main->mouse_pos.y
			- editor->win_main->mouse_pos_prev.y;
		editor->selected_sprite->pos.x += mouse_diff.x / 10;
		editor->selected_sprite->pos.y += mouse_diff.y / 10;
		ui_input_set_text(editor->sprite_x_input,
			ft_b_itoa(editor->selected_sprite->pos.x, temp_str));
		ui_input_set_text(editor->sprite_y_input,
			ft_b_itoa(editor->selected_sprite->pos.y, temp_str));
	}
	get_sprite_ui(editor, editor->selected_sprite);
}

void	sprite_events(t_editor *editor)
{
	if (!editor->selected_wall || !editor->selected_sector)
		return ;
	if (editor->sprite_add_button->state == UI_STATE_CLICK)
	{
		if (!editor->selected_sprite)
			editor->selected_sprite
				= add_sprite_to_wall(editor, editor->selected_wall);
		else
			editor->selected_sprite = NULL;
	}
	else if (editor->sprite_remove_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_sprite)
		{
			remove_sprite(editor, editor->selected_sprite);
			editor->selected_sprite = NULL;
			--editor->selected_wall->sprite_amount;
			ft_printf("Sprite Removed (total : %d)\n",
				editor->selected_wall->sprite_amount);
		}
	}
	select_sprite(editor);
	edit_sprite(editor);
}
