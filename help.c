/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:01 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/11 09:22:20 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Description: Aligns element with position of 'size' inside 'inside'
 * 				with 'gap' amount of pixels inbetween each element;
 *
 * NOTE: this function ignores y/h;
 * 		aka it assumes you have infinite height on 'inside';
 *
 * inside == inside which w and h the output vec2 should fit in;
 * inside.x == width;
 * inside.y == height;
 *
 * size == of the element you want to fit inside 'inside';
*/
t_vec2	align_fill(t_vec2 inside, t_vec4 size, int gap, int i)
{
	int	amount_x;

	amount_x = (inside.x - size.x) / (size.w + gap);
	while ((int)amount_x * (size.w + gap) < inside.x - size.x)
		gap++;
	return (vec2(size.x + (i % (int)amount_x * (size.w + gap)),
			size.y + (i / (int)amount_x * (size.h + gap))));
}

Uint32	random_blue_color(void)
{
	Uint32	color;
	t_rgba	t;

	t.a = 255;
	t.r = rand() % 255;
	t.g = rand() % 255;
	t.b = 245;
	color = rgba_to_hex(t);
	return (color);
}

t_vec2i	conversion(t_editor *editor, t_vec2i v)
{
	t_vec2i	result;

	result.x = (v.x - (int)editor->offset.x)
		* (editor->gap_size * editor->zoom);
	result.y = (v.y - (int)editor->offset.y)
		* (editor->gap_size * editor->zoom);
	return (result);
}

/*
 * NOTE: selection_drop_menu closes when you click something,
 * 		so thats why we have to see if it was last frame clicked;
*/
int	hover_over_open_menus(t_editor *editor)
{
	if (ui_element_is_hover(editor->sector_edit_menu)
		|| ui_element_is_hover(editor->menu_wall_edit)
		|| ui_element_is_hover(editor->sprite_edit_menu)
		|| ui_element_is_hover(editor->texture_menu)
		|| ui_element_is_hover(editor->event_edit_menu)
		|| ui_element_is_hover(editor->spawn_edit_menu)
		|| ui_element_is_hover(editor->entity_edit_menu)
		|| ui_element_was_hover(editor->selection_dropdown_menu)
		|| ui_element_is_hover(editor->menu_toolbox_top))
		return (1);
	return (0);
}

float	get_aspect(float w, float h)
{
	return (w / h);
}
