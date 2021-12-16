/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:52:35 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:52:35 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	zoom_events(t_editor *editor)
{
	t_vec2	last;
	t_vec2	next;

	if (editor->win_main->scroll && !hover_over_open_menus(editor))
	{
		last = vec2(editor->drawing_surface->w / (editor->gap_size
					* editor->zoom), editor->drawing_surface->h
				/ (editor->gap_size * editor->zoom));
		editor->zoom = ft_fclamp(editor->zoom + editor->win_main->scroll
				/ 10.0f, 0.1f, 10.0f);
		next = vec2(editor->drawing_surface->w / (editor->gap_size
					* editor->zoom), editor->drawing_surface->h
				/ (editor->gap_size * editor->zoom));
		editor->offset.x += (last.x - next.x) / 2;
		editor->offset.y += (last.y - next.y) / 2;
		editor->update_grid = 1;
	}
}

/*
 * This calculates the actual position in game for the mouse;
*/
void	calculate_hover(t_editor *editor)
{
	editor->last_mouse_pos = editor->mouse_pos;
	editor->mouse_pos.x = (editor->win_main->mouse_pos.x
			/ (editor->gap_size * editor->zoom)) + (int)editor->offset.x;
	editor->mouse_pos.y = (editor->win_main->mouse_pos.y
			/ (editor->gap_size * editor->zoom)) + (int)editor->offset.y;
	editor->mouse_pos_screen.x = (editor->mouse_pos.x - (int)editor->offset.x)
		* (editor->gap_size * editor->zoom);
	editor->mouse_pos_screen.y = (editor->mouse_pos.y - (int)editor->offset.y)
		* (editor->gap_size * editor->zoom);
	editor->move_amount.x = editor->mouse_pos.x - editor->last_mouse_pos.x;
	editor->move_amount.y = editor->mouse_pos.y - editor->last_mouse_pos.y;
	zoom_events(editor);
}

void	update_info_label(t_editor *editor)
{
	t_rgba	new_col;

	if (editor->info_label->show
		&& SDL_GetTicks() - editor->info_label_start_timer
		>= editor->info_label_timer)
	{
		new_col = hex_to_rgba(ui_label_get_color(editor->info_label));
		new_col.a -= 2;
		ui_label_color_set(editor->info_label, rgba_to_hex(new_col));
		if (new_col.a <= 0)
			editor->info_label->show = 0;
	}
}

void	grid_events(t_editor *editor, SDL_Event e)
{
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
		editor->offset = vec2(0, 0);
	if (editor->win_main->mouse_down == SDL_BUTTON_MIDDLE)
	{
		editor->offset.x -= (editor->win_main->mouse_pos.x
				- editor->win_main->mouse_pos_prev.x)
			/ (editor->gap_size * editor->zoom);
		editor->offset.y -= (editor->win_main->mouse_pos.y
				- editor->win_main->mouse_pos_prev.y)
			/ (editor->gap_size * editor->zoom);
	}
	if (editor->draw_button->was_click)
	{
		editor->selected_sector = NULL;
		editor->selected_entity = NULL;
		editor->selected_event = NULL;
		editor->active_event_elem = NULL;
		editor->selected_sprite = NULL;
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}
}
