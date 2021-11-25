#include "editor.h"

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


