/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 09:46:02 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 09:46:02 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * From in game position calculate the screen position;
*/
void	draw_hover(t_editor *editor)
{
	ui_surface_circle_draw_filled(editor->drawing_surface,
		editor->mouse_pos_screen, 3, 0xffffffff);
	ui_surface_line_draw(editor->drawing_surface,
		vec2i(editor->mouse_pos_screen.x, 0),
		vec2i(editor->mouse_pos_screen.x, editor->drawing_surface->h),
		0x70b0b0b0);
	ui_surface_line_draw(editor->drawing_surface,
		vec2i(0, editor->mouse_pos_screen.y),
		vec2i(editor->drawing_surface->w, editor->mouse_pos_screen.y),
		0x70b0b0b0);
	if (editor->first_point_set)
		ui_surface_line_draw(editor->drawing_surface,
			conversion(editor, editor->first_point),
			editor->mouse_pos_screen, 0xffffff00);
}

/*
 * Draws outline around selected_sector/wall/point/entity;
*/
void	draw_selected(t_editor *editor)
{
	t_vec2i	conv;
	t_vec2i	p1;
	t_vec2i	p2;

	if (editor->selected_sector)
		sector_render(editor, editor->selected_sector, 0xFF00FF00);
	if (editor->selected_point)
		point_render(editor, editor->selected_point, 0xFFFFD700);
	else if (editor->selected_wall)
		wall_render(editor, editor->selected_wall, 0xFFFFD700);
	if (editor->selected_entity)
	{
		conv = conversion(editor, editor->selected_entity->pos);
		p1 = vec2i(conv.x - 10, conv.y - 10);
		p2 = vec2i(conv.x + 10, conv.y + 10);
		ui_surface_rect_draw(editor->drawing_surface, p1, p2, 0xFFFFD700);
	}
}

void	draw_spawn(t_editor *editor)
{
	t_list	*curr;

	ui_surface_circle_draw_filled(editor->drawing_surface,
		conversion(editor, editor->spawn.pos), 10, 0xff00ff00);
	draw_arrow(editor->drawing_surface,
		conversion(editor, editor->spawn.pos), 10, editor->spawn.yaw);
	curr = editor->sectors;
	editor->spawn.inside_sector = NULL;
	while (curr)
	{
		if (check_point_in_sector(curr->content, editor->spawn.pos) == 1)
		{
			editor->spawn.inside_sector = curr->content;
			break ;
		}
		curr = curr->next;
	}
	if (!editor->spawn.inside_sector)
	{
		draw_text(editor->drawing_surface, "Not Inside Sector!",
			conversion(editor, editor->spawn.pos), 0xffff0000);
		editor->errors += 1;
	}
	if (editor->spawn.inside_sector)
		editor->spawn.z = editor->spawn.inside_sector->floor_height;
}

void	update_error_label(t_editor *editor)
{
	char	*temp;

	if (editor->errors > 0)
	{
		temp = ft_sprintf("Consider fixing the %d error(s) before saving!",
				editor->errors);
		ui_label_set_text(editor->error_label, temp);
		ft_strdel(&temp);
		editor->error_label->show = 1;
	}
	else
		editor->error_label->show = 0;
}

void	user_render(t_editor *editor)
{
	editor->errors = 0;
	draw_grid(editor);
	draw_sectors(editor, editor->sectors);
	draw_hover(editor);
	draw_selected(editor);
	draw_entities_yaw(editor, editor->entities);
	draw_spawn(editor);
	draw_drawing_wall_len(editor);
	SDL_UpdateTexture(editor->drawing_texture, NULL,
		editor->drawing_surface->pixels, editor->drawing_surface->pitch);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer,
		editor->drawing_texture, NULL, NULL);
	draw_entities(editor, editor->entities);
	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
	SDL_FillRect(editor->drawing_surface, NULL, 0);
	update_error_label(editor);
}
