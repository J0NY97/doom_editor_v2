/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_visualizer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:41:19 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/17 16:30:03 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	draw_wall_on_surface(SDL_Surface *surface, t_wall *wall, float scale)
{
	int		amount_x;
	int		amount_y;
	int		ii;
	int		jj;

	amount_x = ceil(wall->width / wall->texture_scale);
	amount_y = ceil(wall->height / wall->texture_scale);
	jj = -1;
	while (++jj <= amount_y)
	{
		ii = -1;
		while (++ii <= amount_x)
			SDL_BlitScaled(wall->texture, NULL, surface,
				&(SDL_Rect){ii * scale, jj * scale, scale, scale});
	}
}

/*
 * SDL_Surface	*texture; // DONT FREE;
 * int size; this is a unit of measurement; (...no s**t (hard to explain));
*/
void	draw_sprites_on_surface(
		t_editor *editor, SDL_Surface *surface, t_list *sprites, float aspect)
{
	t_sprite	*sprite;
	t_vec4i		xywh;
	SDL_Surface	*texture;
	int			size;

	size = 64;
	while (sprites)
	{
		sprite = sprites->content;
		texture = editor->wall_textures[sprite->texture_id];
		if (sprite->type == STATIC)
			xywh = vec4i(0, 0, texture->w, texture->h);
		else
			xywh = vec4i(0, 0, 64, 64);
		sprite->screen_pos.w = (size * sprite->scale) * aspect;
		sprite->screen_pos.h
			= (float)xywh.h * ((float)sprite->screen_pos.w / (float)xywh.w);
		sprite->screen_pos.x = (int)sprite->pos.x * size * aspect;
		sprite->screen_pos.y = (int)sprite->pos.y * size * aspect;
		SDL_BlitScaled(texture, &(SDL_Rect){xywh.x, xywh.y, xywh.w, xywh.h},
			surface, &(SDL_Rect){sprite->screen_pos.x, sprite->screen_pos.y,
			sprite->screen_pos.w, sprite->screen_pos.h});
		sprites = sprites->next;
	}
}

void	draw_selected_sprite(t_editor *editor, SDL_Surface *surface)
{
	t_vec2i	p1;
	t_vec2i	p2;

	if (editor->selected_sprite)
	{
		p1 = vec2i(editor->selected_sprite->screen_pos.x,
				editor->selected_sprite->screen_pos.y);
		p2 = vec2i(p1.x + editor->selected_sprite->screen_pos.w,
				p1.y + editor->selected_sprite->screen_pos.h);
		ui_surface_rect_draw(surface, p1, p2, 0xFFFFD700);
	}
}

void	visualize_wall(t_editor *editor, t_wall *wall)
{
	SDL_Surface	*surface;
	float		aspect;
	float		scale;
	int			size;

	SDL_SetRenderTarget(editor->win_main->renderer,
		editor->wall_render->texture);
	ui_texture_fill(editor->win_main->renderer,
		editor->wall_render->texture, 0xff000000);
	size = 64;
	aspect = get_ratio_f(vec2(wall->width * size, wall->height * size),
			vec2(editor->wall_render->pos.w, editor->wall_render->pos.h));
	scale = (size * wall->texture_scale) * aspect;
	surface = ui_surface_new(wall->width * size * aspect,
			wall->height * size * aspect);
	wall->texture = editor->wall_textures[wall->wall_texture];
	draw_wall_on_surface(surface, wall, scale);
	draw_sprites_on_surface(editor, surface, wall->sprites, aspect);
	draw_selected_sprite(editor, surface);
	ui_surface_draw_border(surface, 1, 0xff00ff00);
	SDL_UpdateTexture(editor->wall_render->texture, &(SDL_Rect){0, 0,
		ft_min(surface->w, editor->wall_render->current_texture_size.x),
		ft_min(surface->h, editor->wall_render->current_texture_size.y)},
		surface->pixels, surface->pitch);
	SDL_FreeSurface(surface);
}
