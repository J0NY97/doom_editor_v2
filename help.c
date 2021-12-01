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
	return (vec2(size.x + (i % (int)(amount_x) * (size.w + gap)),
		size.y + (i / (int)(amount_x) * (size.h + gap))));
}

Uint32	random_blue_color(void)
{
	Uint32	color;

	color = rgba_to_hex((t_rgba){
			.a = 255, .r = rand() % 255,
			.g = rand() % 255, .b = 245});
	return (color);
}

t_vec2i	conversion(t_editor *editor, t_vec2i v)
{
	t_vec2i	result;

	result.x = (v.x - editor->offset.x) * (editor->gap_size * editor->zoom);
	result.y = (v.y - editor->offset.y) * (editor->gap_size * editor->zoom);
	return (result);
}

/*
 * NOTE: selection_drop_menu closes when you click something so thats why we have to see if it was last frame clicked;
*/
int	hover_over_open_menus(t_editor *editor)
{
	if (ui_element_is_hover(editor->sector_edit_menu)
		|| ui_element_is_hover(editor->menu_wall_edit)
		|| ui_element_is_hover(editor->sprite_edit_menu)
		|| ui_element_is_hover(editor->texture_menu)
		|| ui_element_is_hover(editor->event_edit_menu)
		|| ui_element_was_hover(editor->selection_dropdown_menu)
		|| ui_element_is_hover(editor->menu_toolbox_top))
		return (1);
	return (0);
}

float	get_aspect(float w, float h)
{
	return (w / h);
}

/*
 * Font should already be opened before this function call;
*/
void	draw_text(SDL_Surface *surface, char *text, TTF_Font *font, t_vec2i pos, Uint32 color)
{
	SDL_Surface	*text_surface;
	t_rgba		rgba;

	if (font)
	{
		rgba = hex_to_rgba(color);
		TTF_SetFontHinting(font, TTF_HINTING_MONO);
		text_surface = TTF_RenderText_Blended(font, text,
				(SDL_Color){rgba.r, rgba.g, rgba.b, rgba.a});
		SDL_BlitSurface(text_surface, NULL, surface, 
			&(SDL_Rect){pos.x - (text_surface->w / 2),
			pos.y - (text_surface->h / 2),
			text_surface->w, text_surface->h});
		SDL_FreeSurface(text_surface);
	}
	else
		ft_printf("[%s] Failed drawing text \"%s\" no font.\n", __FUNCTION__, text);
}
