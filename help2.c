/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:09:06 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 14:09:06 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Font should already be opened before this function call;
 * NOTE:
 * 	This function will draw the text in the middle of 'pos', no matter what
 * 		length the text is. (meaning the 'pos' will be in the middle of the
 * 		drawn text, not in the top left)
*/
void	draw_text(SDL_Surface *surface, char *text, t_vec2i pos, Uint32 color)
{
	t_rgba			rgba;
	SDL_Surface		*text_surface;
	static TTF_Font	*font = NULL;

	if (!font)
		font = TTF_OpenFont(UI_PATH"fonts/DroidSans.ttf", 20);
	if (font)
	{
		rgba = hex_to_rgba(color);
		text_surface = TTF_RenderText_Blended(font, text,
				(SDL_Color){rgba.r, rgba.g, rgba.b, rgba.a});
		SDL_BlitSurface(text_surface, NULL, surface,
			&(SDL_Rect){pos.x - (text_surface->w / 2),
			pos.y - (text_surface->h / 2),
			text_surface->w, text_surface->h});
		SDL_FreeSurface(text_surface);
	}
	else
		ft_printf("[%s] Error : somehow \"%s\" no font.\n", __FUNCTION__, text);
}

void	set_elem_parent_and_recipe(
		t_ui_element *elem, int ui_type, t_ui_element *parent, char *recipe_id)
{
	g_acceptable[ui_type].maker(parent->win, elem);
	ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
	ui_element_edit(elem, ui_layout_get_recipe(parent->win->layout, recipe_id));
	elem->free_me = 0;
}

void	draw_arrow(SDL_Surface *surface, t_vec2i start, int len, float yaw)
{
	float	angle;
	t_vec2i	p1;
	t_vec2i	p2;

	angle = yaw * (M_PI / 180);
	p1 = start;
	p2 = vec2i(cos(angle) * (len * 2) + p1.x, sin(angle) * (len * 2) + p1.y);
	ui_surface_line_draw(surface, p1, p2, 0xffaaab5d);
	angle = (yaw - 45) * (M_PI / 180);
	p1 = vec2i(cos(angle) * -len + p2.x, sin(angle) * -len + p2.y);
	ui_surface_line_draw(surface, p2, p1, 0xffaaab5d);
	angle = (yaw + 45) * (M_PI / 180);
	p1 = vec2i(cos(angle) * -len + p2.x, sin(angle) * -len + p2.y);
	ui_surface_line_draw(surface, p2, p1, 0xffaaab5d);
}

t_vec2i	get_middle(t_vec2i p1, t_vec2i p2)
{
	return (vec2i((p1.x + p2.x) / 2, (p1.y + p2.y) / 2));
}

/*
 * Returns last t_list in 'list';
*/
t_list	*get_last_tlist(t_list *list)
{
	while (list->next)
		list = list->next;
	return (list);
}
