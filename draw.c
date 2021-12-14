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

void	draw_grid_lines(SDL_Surface *surface, float gap_size, float zoom)
{
	int			w_amount;
	int			h_amount;
	int			i;

	w_amount = surface->w / (gap_size * zoom);
	h_amount = surface->h / (gap_size * zoom);
	i = 0;
	while (i < w_amount || i < h_amount)
	{
		if (i < w_amount)
			ui_surface_line_draw(surface, vec2i(i * (gap_size * zoom), 0),
				vec2i(i * (gap_size * zoom), surface->h), 0xff353535);
		if (i < h_amount)
			ui_surface_line_draw(surface, vec2i(0, i * (gap_size * zoom)),
				vec2i(surface->w, i * (gap_size * zoom)), 0xff353535);
		i++;
	}
}

void	draw_grid(t_editor *editor)
{
	if (editor->update_grid)
	{
		SDL_FillRect(editor->grid_surface, NULL, 0);
		draw_grid_lines(editor->grid_surface, editor->gap_size, editor->zoom);
		editor->update_grid = 0;
		ft_printf("[%s] We are updating grid surface.\n", __FUNCTION__);
	}
	SDL_BlitSurface(editor->grid_surface, NULL, editor->drawing_surface, NULL);
}

void	draw_sectors(t_editor *editor, t_list *sectors)
{
	t_sector	*sector;
	char		temp[20];
	t_sector	*hovered_this_frame;
	Uint32		color;

	hovered_this_frame = NULL;
	while (sectors)
	{
		sector = sectors->content;
		sort_walls(sector->walls);
		sector_render(editor, sector, sector->color);
		sector->center = get_sector_center(sector);
		sector->screen_center = conversion(editor, sector->center);
		color = 0xffffffff;
		if (sector == editor->selected_sector)
			color = 0xff00ff00;
		draw_text(editor->drawing_surface, ft_b_itoa(sector->id, temp),
			sector->screen_center, color);
		sector_check_errors(editor, sector);
		if (vec2_in_vec4(editor->win_main->mouse_pos,
				vec4i(sector->screen_center.x - 10,
					sector->screen_center.y - 10, 20, 20)))
			hovered_this_frame = sector;
		sectors = sectors->next;
	}
	editor->hovered_sector = hovered_this_frame;
}

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

void	draw_entities_yaw(t_editor *editor, t_list *entities)
{
	t_entity	*entity;

	while (entities)
	{
		entity = entities->content;
		draw_arrow(editor->drawing_surface,
			conversion(editor, entity->pos), 10, entity->yaw);
		entity->inside_sector
			= point_inside_which_sector(editor->sectors, entity->pos);
		entity_check_errors(editor, entity);
		entities = entities->next;
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

void	draw_entities(t_editor *editor, t_list *entities)
{
	while (entities)
	{
		entity_render(editor, entities->content);
		entities = entities->next;
	}
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

void	draw_drawing_wall_len(t_editor *editor)
{
	char	temp_str[20];
	int		len ;
	t_vec2i	middle;

	if (editor->first_point_set)
	{
		len = distance(editor->first_point.v, editor->mouse_pos.v, 2);
		middle = conversion(editor,
				get_middle(editor->first_point, editor->mouse_pos));
		draw_text(editor->drawing_surface, ft_b_itoa(len, temp_str),
			middle, 0xffdeface);
	}
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
