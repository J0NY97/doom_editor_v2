#include "libui.h"

SDL_Texture	*ui_create_texture(SDL_Renderer *renderer, t_vec2i pos)
{
	SDL_Texture	*texture;

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, pos.x, pos.y);
	return (texture);
}

void	ui_texture_fill(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color)
{
	t_rgba		rgba;

	rgba = hex_to_rgba(color);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderFillRect(renderer, NULL);
	SDL_SetRenderTarget(renderer, NULL);
}

void	ui_texture_fill_rect(SDL_Renderer *renderer, SDL_Texture *texture, Uint32 color, t_vec4i rect)
{
	t_rgba		rgba;

	rgba = hex_to_rgba(color);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderFillRect(renderer, &(SDL_Rect){rect.x, rect.y, rect.w, rect.h});
	SDL_SetRenderTarget(renderer, NULL);
}

void	ui_texture_draw_border(SDL_Renderer *renderer, SDL_Texture *texture, size_t thicc, Uint32 color)
{
	t_rgba		rgba;
	SDL_Rect	rect;
	size_t		i;
	int			w;
	int			h;

	i = -1;
	rgba = hex_to_rgba(color);
	SDL_SetRenderTarget(renderer, texture);
	SDL_SetRenderDrawColor(renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	while (++i < thicc)
	{
		rect.x = i;
		rect.y = i;
		rect.w = w - i - 1;
		rect.h = h - i - 1;
		SDL_RenderDrawRect(renderer, &rect);
	}
	SDL_SetRenderTarget(renderer, NULL);
}

SDL_Surface	*ui_surface_create_from_text_recipe(t_ui_label *recipe)
{
	SDL_Surface	*surface;
	t_rgba		rgba;
	SDL_Color	color;

	// First try to find with absolute path...
	if (!recipe->font || recipe->font_recreate)
	{
		if (recipe->font)
			TTF_CloseFont(recipe->font);
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		recipe->font_recreate = 0;
		ft_printf("[%s] Font recretead.\n", __FUNCTION__);
	}
	// ... if not found, try to find from UI_FONT_PATH/"font_name" ...
	if (!recipe->font)
	{
		char	*temp_font_path;
		temp_font_path = ft_strdup(recipe->font_path);
		ft_printf("[%s] 2nd try : %s, trying to find from libui fonts. %s%s\n", __FUNCTION__, TTF_GetError(), UI_FONT_PATH, recipe->font_path);
		if (recipe->font_path)
			ft_strdel(&recipe->font_path);
		recipe->font_path = ft_strjoin(UI_FONT_PATH, temp_font_path);
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		if (!recipe->font)
		{
			ft_printf("[%s] Well apparently that didn\'t work either.\n", __FUNCTION__);
			return (NULL);
		}
	}
	// ... if still not found, try to find default font. IF this is not found, its the dev's fault.
	if (!recipe->font)
	{
		ft_printf("[%s] 3rd try : %s, defaulting to %sDroidSans.ttf\n", __FUNCTION__, TTF_GetError(), UI_FONT_PATH);
		if (recipe->font_path)
			ft_strdel(&recipe->font_path);
		recipe->font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		if (!recipe->font)
		{
			ft_printf("[%s] Well apparently that didn\'t work either.\n", __FUNCTION__);
			return (NULL);
		}
	}
	TTF_SizeUTF8(recipe->font, recipe->text, &recipe->text_wh.x, &recipe->text_wh.y);
	rgba = hex_to_rgba(recipe->font_color);
	color.r = rgba.r;
	color.g = rgba.g;
	color.b = rgba.b;
	color.a = rgba.a;
	if (recipe->max_w == -1)
		surface = TTF_RenderUTF8_Blended(recipe->font, recipe->text, color);
	else
		surface = TTF_RenderUTF8_Blended_Wrapped(recipe->font, recipe->text, color, recipe->max_w);
	if (!surface)
		ft_printf("[%s] Failed creating surface.\n", __FUNCTION__);
	return (surface);
}

SDL_Texture	*ui_texture_create_from_text_recipe(SDL_Renderer *renderer, t_ui_label *recipe)
{
	SDL_Texture	*texture;
	SDL_Surface	*surface;
	t_rgba		rgba;
	SDL_Color	color;

	// First try to find with absolute path...
	if (!recipe->font || recipe->font_recreate)
	{
		if (recipe->font)
			TTF_CloseFont(recipe->font);
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		recipe->font_recreate = 0;
		ft_printf("[%s] Font recretead.\n", __FUNCTION__);
	}
	// ... if not found, try to find from UI_FONT_PATH/"font_name" ...
	if (!recipe->font)
	{
		char	*temp_font_path;
		temp_font_path = ft_strdup(recipe->font_path);
		ft_printf("[ui_texture_create_from_text_recipe] 2nd try : %s, trying to find from libui fonts. %s%s\n", TTF_GetError(), UI_FONT_PATH, recipe->font_path);
		if (recipe->font_path)
			ft_strdel(&recipe->font_path);
		recipe->font_path = ft_strjoin(UI_FONT_PATH, temp_font_path);
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		if (!recipe->font)
		{
			ft_printf("[ui_texture_create_from_text_recipe] Well apparently that didn\'t work either.\n");
			return (NULL);
		}
	}
	// ... if still not found, try to find default font. IF this is not found, its the dev's fault.
	if (!recipe->font)
	{
		ft_printf("[ui_texture_create_from_text_recipe] 3rd try : %s, defaulting to %sDroidSans.ttf\n", TTF_GetError(), UI_FONT_PATH);
		if (recipe->font_path)
			ft_strdel(&recipe->font_path);
		recipe->font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
		recipe->font = TTF_OpenFont(recipe->font_path, recipe->font_size);
		if (!recipe->font)
		{
			ft_printf("[ui_texture_create_from_text_recipe] Well apparently that didn\'t work either.\n");
			return (NULL);
		}
	}
	TTF_SizeUTF8(recipe->font, recipe->text, &recipe->text_wh.x, &recipe->text_wh.y);
	rgba = hex_to_rgba(recipe->font_color);
	color.r = rgba.r;
	color.g = rgba.g;
	color.b = rgba.b;
	color.a = rgba.a;
	if (recipe->max_w == -1)
		surface = TTF_RenderUTF8_Blended(recipe->font, recipe->text, color);
	else
		surface = TTF_RenderUTF8_Blended_Wrapped(recipe->font, recipe->text, color, recipe->max_w);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	return (texture);
}
