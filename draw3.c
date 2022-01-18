/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:11:41 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:11:41 by jsalmi           ###   ########.fr       */
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
		LG_DEBUG("[%s] We are updating grid surface.", __FUNCTION__);
	}
	SDL_BlitSurface(editor->grid_surface, NULL, editor->drawing_surface, NULL);
}
