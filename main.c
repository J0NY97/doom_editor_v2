#include "editor.h"

void	calculate_hover(t_editor *editor)
{
	editor->mouse_pos.x = editor->win_main->mouse_pos.x / (10.0f * editor->zoom);
	editor->mouse_pos.y = editor->win_main->mouse_pos.y / (10.0f * editor->zoom);
}

void	draw_hover(t_editor *editor)
{
	t_vec2i	convert_back;

	convert_back.x = editor->mouse_pos.x * (editor->gap_size * editor->zoom);
	convert_back.y = editor->mouse_pos.y * (editor->gap_size * editor->zoom);
	ui_surface_circle_draw_filled(editor->drawing_surface, convert_back, 3, 0xffffffff);
}

void	user_events(t_editor *editor, SDL_Event e)
{
	calculate_hover(editor);
	if (editor->win_main->mouse_pos.y < 70) // the 70 comes from the menu_toolbox.h
		return ;
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
		editor->offset = vec2i(0, 0);
	if (editor->win_main->mouse_down == SDL_BUTTON_MIDDLE)
	{
		editor->offset.x -= (editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x);
		editor->offset.y -= (editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y);
	}
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
		{
			t_point	*point;
			point = ft_memalloc(sizeof(t_point));
			point->pos.x = editor->mouse_pos.x + editor->offset.x;
			point->pos.y = editor->mouse_pos.y + editor->offset.y;
			add_to_list(&editor->points, point, sizeof(t_point));
		}
	}
}

void	draw_grid(SDL_Surface *surface, float gap_size, float zoom)
{
	int		w_amount;
	int		h_amount;
	int		i;

	w_amount = surface->w / (gap_size * zoom);
	h_amount = surface->h / (gap_size * zoom);
	i = 0;
	while (i < w_amount || i < h_amount)
	{
		if (i < w_amount)
			ui_surface_line_draw(surface, vec2i(i * (gap_size * zoom), 0), vec2i(i * (gap_size * zoom), surface->h), 0xff353535);
		if (i < h_amount)
			ui_surface_line_draw(surface, vec2i(0, i * (gap_size * zoom)), vec2i(surface->w, i * (gap_size * zoom)), 0xff353535);
		i++;
	}
}

void	draw_points(SDL_Surface *surface, t_list *points, float gap_size, float zoom, t_vec2i offset)
{
	int	i;

	i = -1;
	while (points)
	{
		ft_printf("Point #%d\n", ++i);
		point_render(surface, points->content, gap_size, zoom, offset);
		points = points->next;
	}
}

void	user_render(t_editor *editor)
{
	draw_grid(editor->drawing_surface, editor->gap_size, editor->zoom);
	draw_points(editor->drawing_surface, editor->points, editor->gap_size, editor->zoom, editor->offset); // remove this at some point;
	draw_hover(editor);

	SDL_UpdateTexture(editor->drawing_texture, NULL, editor->drawing_surface->pixels, editor->drawing_surface->pitch);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer, editor->drawing_texture, NULL, NULL);
	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
	SDL_FillRect(editor->drawing_surface, NULL, 0);
}

void	editor_init(t_editor *editor)
{
	ui_layout_load(&editor->layout, "layout.ui");
	editor->win_main = ui_list_get_window_by_id(editor->layout.windows, "win_main");
	editor->draw_button = ui_list_get_element_by_id(editor->layout.elements, "draw_button");
}

void	draw_init(t_editor *editor)
{
	editor->drawing_surface = ui_surface_new(1920, 1030);
	editor->drawing_texture = SDL_CreateTextureFromSurface(editor->win_main->renderer, editor->drawing_surface);
	editor->gap_size = 10.0f;
	editor->zoom = 1.0f;
	editor->points = NULL;
	editor->offset = vec2i(0, 0);
}

int	main(int ac, char **av)
{
	t_editor	editor;
	int			run;
	SDL_Event	e;

	ui_sdl_init();
	editor_init(&editor);
	draw_init(&editor);

	run = 1;
	while (run)
	{
		if (editor.win_main->wants_to_close)
			run = 0;
		while (SDL_PollEvent(&e))
		{
			ui_layout_event(&editor.layout, e);

			// User Events
			user_events(&editor, e);
		}


		// User Render
		user_render(&editor);

		ui_layout_render(&editor.layout);
	}
}
