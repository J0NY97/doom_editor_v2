#include "libui.h"

void	ui_window_new(t_ui_window *win, char *title, t_vec4 pos)
{
	memset(win, 0, sizeof(t_ui_window));
	if (!title)
		win->title = ft_strdup("Window Title Not Set");
	else
		win->title = ft_strdup(title);
	win->pos = pos;
	win->win = SDL_CreateWindow(win->title, win->pos.x, win->pos.y, win->pos.w, win->pos.h, 0);
	win->renderer = SDL_CreateRenderer(win->win, -1, SDL_RENDERER_ACCELERATED);
	win->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win->pos.w, win->pos.h);
	win->show = 1;
	win->children = NULL;

	win->window_id = SDL_GetWindowID(win->win);

	SDL_GetWindowSize(win->win, &win->screen_pos.w, &win->screen_pos.h);
	win->texture_scale = vec2(win->pos.w / win->screen_pos.w, win->pos.h / win->screen_pos.h);
	SDL_GetMouseState(&win->window_mouse_pos.x, &win->window_mouse_pos.y);
	win->mouse_pos.x = win->window_mouse_pos.x * win->texture_scale.x;
	win->mouse_pos.y = win->window_mouse_pos.y * win->texture_scale.y; 
	win->bg_color = 0xff000000;
}

void	ui_window_event(t_ui_window *win, SDL_Event e)
{
	if (win->window_id == e.window.windowID)
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				SDL_GetWindowSize(win->win, &win->screen_pos.w, &win->screen_pos.h);
				win->texture_scale = vec2(win->pos.w / win->screen_pos.w, win->pos.h / win->screen_pos.h);
				win->textures_recreate = 1;
			}
			else if (e.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				if (win->hide_on_x)
					ui_window_flag_set(win, UI_WINDOW_HIDE);
				else
					win->wants_to_close = 1;
			}
		}
		else if (e.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&win->window_mouse_pos.x, &win->window_mouse_pos.y);
			win->mouse_pos.x = win->window_mouse_pos.x * win->texture_scale.x;
			win->mouse_pos.y = win->window_mouse_pos.y * win->texture_scale.y;
		}
		else if (e.type == SDL_MOUSEWHEEL)
			win->scroll = e.wheel.y;
		else if (e.type == SDL_KEYDOWN)
		{
			int modkey = SDL_GetModState();
			if (modkey & KMOD_LALT) // LALT mods
			{
				if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) // make fullscreen
				{
					ui_window_flag_set(win, UI_WINDOW_FULLSCREEN);
					win->textures_recreate = 1;
				}
			}
			else if (modkey & KMOD_LCTRL) // LCTRL mods
			{
				if (e.key.keysym.scancode == SDL_SCANCODE_R) // force texture recreate
					win->textures_recreate = 1;
			}
		}
		if (e.button.type == SDL_MOUSEBUTTONDOWN)
		{
			win->mouse_down = e.button.button;
			win->mouse_down_last_frame = win->mouse_down;
		}
	}
	if (e.button.type == SDL_MOUSEBUTTONUP)
		win->mouse_down = 0;
}

int	ui_window_render(t_ui_window *win)
{
	t_rgba	rgba;

	SDL_SetRenderTarget(win->renderer, NULL);
	SDL_RenderCopy(win->renderer, win->texture, NULL, NULL);

	SDL_SetRenderTarget(win->renderer, win->texture);
	SDL_RenderClear(win->renderer);
	rgba = hex_to_rgba(win->bg_color);
	SDL_SetRenderDrawColor(win->renderer, rgba.r, rgba.g, rgba.b, rgba.a);
	SDL_RenderFillRect(win->renderer, NULL);

	SDL_SetRenderTarget(win->renderer, NULL);
	win->textures_recreate = 0;


	// Resetting events, becuase sdl wont go inside the event function,if NO events have been done.
	win->mouse_pos_prev = win->mouse_pos;
	win->scroll = 0;
	win->mouse_down_last_frame = 0;
	return (1);
}

void	ui_window_free(void *win)
{
	(void)win;
}

/*
 * Window edititing functions down from here.
*/

void	ui_window_texture_redo(t_ui_window *win)
{
	if (win->texture)
		SDL_DestroyTexture(win->texture);
	win->texture = SDL_CreateTexture(win->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, win->pos.w, win->pos.h);
}

void	ui_window_pos_set(t_ui_window *win, t_vec4 pos)
{
	win->pos = pos;
	SDL_SetWindowPosition(win->win, win->pos.x, win->pos.y);
	SDL_SetWindowSize(win->win, win->pos.w, win->pos.h);
	ui_window_texture_redo(win);
	SDL_GetWindowSize(win->win, &win->screen_pos.w, &win->screen_pos.h);
	win->texture_scale = vec2(win->pos.w / win->screen_pos.w, win->pos.h / win->screen_pos.h);
}

void	ui_window_flag_set(t_ui_window *win, int flags)
{
	int	curr_flags;

	curr_flags = SDL_GetWindowFlags(win->win);
	if (flags & UI_WINDOW_HIDE)
		SDL_HideWindow(win->win);
	if (flags & UI_WINDOW_SHOW)
		SDL_ShowWindow(win->win);
	if (flags & UI_WINDOW_MAXIMIZE)
		SDL_MaximizeWindow(win->win);
	if (flags & UI_WINDOW_MINIMIZE)
		SDL_MinimizeWindow(win->win);
	if (flags & UI_WINDOW_FULLSCREEN)
	{
		if (curr_flags & SDL_WINDOW_FULLSCREEN)
			SDL_SetWindowFullscreen(win->win, 0);
		else
			SDL_SetWindowFullscreen(win->win, SDL_WINDOW_FULLSCREEN);
	}
	if (flags & UI_WINDOW_FULLSCREEN_WINDOWED)
		SDL_SetWindowFullscreen(win->win, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (flags & UI_WINDOW_GRAB)
		SDL_SetWindowGrab(win->win, SDL_TRUE);
	if (flags & UI_WINDOW_RESIZEABLE)
		SDL_SetWindowResizable(win->win, SDL_TRUE);
}

void	ui_window_title_set(t_ui_window *win, const char *title)
{
	if (win->title)
		ft_strdel(&win->title);
	win->title = ft_strdup(title);
	SDL_SetWindowTitle(win->win, title);
}

void	ui_window_id_set(t_ui_window *win, const char *id)
{
	if (!id)
		return ;
	if (win->id)
		ft_strdel(&win->id);
	win->id = ft_strdup(id);
}

/*
 * This function should probably be called right after loading the ui,
 * otherwise you might save the ui_window to a variable that is used somewhere else,
 * or renderer...
*/
void	ui_window_replace_win(t_ui_window *ui_win, SDL_Window *sdl_win)
{
	if (ui_win->win)
		SDL_DestroyWindow(ui_win->win);
	ui_win->win = sdl_win;
	if (ui_win->renderer)
		SDL_DestroyRenderer(ui_win->renderer);
	ui_win->renderer = SDL_GetRenderer(sdl_win);
	if (!ui_win->renderer)
		ui_win->renderer = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);
	else
		ft_printf("[%s] The new window already had a renderer associated with it.\n", __FUNCTION__);
	if (ui_win->texture)
		SDL_DestroyTexture(ui_win->texture);
	ui_win->texture = SDL_CreateTexture(ui_win->renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET, ui_win->pos.w, ui_win->pos.h);
	ui_win->window_id = SDL_GetWindowID(sdl_win);
}
