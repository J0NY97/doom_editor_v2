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

	// Draw from first point to where you're hovering;
	if (editor->first_point_set)
		ui_surface_line_draw(editor->drawing_surface,
			conversion(editor, editor->first_point),
			convert_back, 0xffffff00);
}

void	user_events(t_editor *editor, SDL_Event e)
{
	t_vec2i	actual_pos;
	t_vec2i	move_amount;

	calculate_hover(editor);
	actual_pos.x = editor->mouse_pos.x + editor->offset.x;
	actual_pos.y = editor->mouse_pos.y + editor->offset.y;
	move_amount.x = editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x;
	move_amount.y = editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y;
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
			if (!editor->selected_sector)
			{
				editor->selected_sector = ft_memalloc(sizeof(t_sector));
				editor->selected_sector->id = ++editor->sector_amount;
				editor->selected_sector->color = random_blue_color();
				add_to_list(&editor->sectors, editor->selected_sector, sizeof(t_sector));
			}
			if (editor->selected_sector)
			{
				if (!editor->first_point_set)
				{
					editor->first_point_set = 1;
					editor->first_point = actual_pos;
				}
				else if (!editor->second_point_set)
				{
					editor->second_point_set = 1;
					editor->second_point = actual_pos;
				}
				if (editor->first_point_set && editor->second_point_set)
				{
					t_wall	*wall;
					wall = ft_memalloc(sizeof(t_wall));
					wall->p1 = ft_memalloc(sizeof(t_point));
					wall->p1->pos = editor->first_point;
					wall->p2 = ft_memalloc(sizeof(t_point));
					wall->p2->pos = editor->second_point;
					editor->first_point_set = 0;
					editor->second_point_set = 0;
					++editor->selected_sector->wall_amount;
					add_to_list(&editor->points, wall->p1, sizeof(t_point));
					add_to_list(&editor->points, wall->p2, sizeof(t_point));
					add_to_list(&editor->walls, wall, sizeof(t_wall));
					add_to_list(&editor->selected_sector->walls, wall, sizeof(t_wall));
					if (!editor->selected_sector->first_point_set)
					{
						editor->selected_sector->first_point = editor->first_point;
						editor->selected_sector->first_point_set = 1;
					}
					if (compare_veci(editor->second_point.v, editor->selected_sector->first_point.v, 2))
						editor->selected_sector = NULL;
					else
					{
						editor->first_point = editor->second_point;
						editor->first_point_set = 1;
					}
				}
			}
		}
	}
	else if (editor->point_button->state == UI_STATE_CLICK)
	{
		editor->selected_wall = NULL;
		editor->selected_sector = NULL;
		editor->selected_entity = NULL;
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
			editor->selected_point = get_point_from_list_around_radius(editor->points, actual_pos, 1.0f);
		else if (editor->selected_point && editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
			editor->selected_point->pos = vec2i_add(editor->selected_point->pos, move_amount);
	}
	else if (editor->wall_button->state == UI_STATE_CLICK)
	{
		editor->selected_point = NULL;
		editor->selected_sector = NULL;
		editor->selected_entity = NULL;
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
			editor->selected_wall = get_wall_from_list_around_radius(editor->walls, actual_pos, 1.0f);
		else if (editor->selected_wall && editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		{
			editor->selected_wall->p1->pos = vec2i_add(editor->selected_wall->p1->pos, move_amount);
			editor->selected_wall->p2->pos = vec2i_add(editor->selected_wall->p2->pos, move_amount);
		}
	}
	else if (editor->sector_button->state == UI_STATE_CLICK)
	{
		editor->selected_point = NULL;
		editor->selected_wall = NULL;
		editor->selected_entity = NULL;
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
			editor->selected_sector = get_sector_from_list_around_radius(editor->sectors, actual_pos, 1);
		else if (editor->selected_sector && editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		{
			t_list	*wall_list;
			t_wall	*wall;

			wall_list = editor->selected_sector->walls;
			while (wall_list)
			{
				wall = wall_list->content;
				wall->p1->pos = vec2i_add(wall->p1->pos, move_amount);
				wall->p2->pos = vec2i_add(wall->p2->pos, move_amount);
				wall_list = wall_list->next;
			}
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

void	draw_points(t_editor *editor, t_list *points, Uint32 color)
{
	int	i;

	i = -1;
	while (points)
	{
		point_render(editor, points->content, color);
		points = points->next;
	}
}

void	draw_walls(t_editor *editor, t_list *walls, Uint32 color)
{
	t_wall	*wall;

	while (walls)
	{
		wall = walls->content;
		wall_render(editor, wall, color);
		point_render(editor, wall->p1, color);
		wall->middle = get_wall_middle(wall);
		ui_surface_line_draw(editor->drawing_surface, wall->middle, );
		walls = walls->next;
	}
}

/*
 * Font should already be opened before this function call;
*/
void	draw_text(SDL_Surface *surface, char *text, TTF_Font *font, t_vec2i pos)
{
	SDL_Surface	*text_surface;

	if (font)
	{
		text_surface = TTF_RenderText_Blended(font, text, (SDL_Color){255, 255, 255, 255});
		SDL_BlitSurface(text_surface, NULL, surface, 
			&(SDL_Rect){pos.x - (text_surface->w / 2), pos.y - (text_surface->h / 2),
			text_surface->w, text_surface->h});
		SDL_FreeSurface(text_surface);
	}
	else
		ft_printf("[%s] Failed drawing text \"%s\" no font.\n", __FUNCTION__, text);
}

void	draw_sector_number(t_editor *editor, t_sector *sector)
{
	char	temp[20];

	ft_b_itoa(sector->id, temp);
	draw_text(editor->drawing_surface, temp, editor->font, conversion(editor, sector->center));
}

t_vec2i	get_sector_center(t_editor *editor, t_sector *sector)
{
	int		i;
	float	x;
	float	y;
	t_list	*wall;
	t_wall	*w;

	x = 0;
	y = 0;
	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		x += (w->p1->pos.x + w->p2->pos.x);
		y += (w->p1->pos.y + w->p2->pos.y);
		wall = wall->next;
	}
	i = ft_lstlen(sector->walls) * 2;
	if (i < 2)
		return (vec2i(-1, -1));
	return (vec2i(x / i, y / i));
}

void	draw_sectors(t_editor *editor, t_list *sectors)
{
	t_sector	*sector;

	while (sectors)
	{
		sector = sectors->content;
		sector_render(editor, sector, sector->color);
		sector->center = get_sector_center(editor, sector);
		draw_sector_number(editor, sector);
		sectors = sectors->next;
	}
}

void	draw_selected(t_editor *editor)
{
	if (editor->selected_point)
		point_render(editor, editor->selected_point, 0xff00ff00);
	else if (editor->selected_wall)
		wall_render(editor, editor->selected_wall, 0xff00ff00);
	else if (editor->selected_sector)
		sector_render(editor, editor->selected_sector, 0xff00ff00);
}

void	user_render(t_editor *editor)
{
	draw_grid(editor->drawing_surface, editor->gap_size, editor->zoom);
	draw_points(editor, editor->points, 0xff00ff00); // remove this at some point;
	draw_sectors(editor, editor->sectors);
	draw_hover(editor);
	draw_selected(editor);

	SDL_UpdateTexture(editor->drawing_texture, NULL, editor->drawing_surface->pixels, editor->drawing_surface->pitch);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer, editor->drawing_texture, NULL, NULL);
	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
	SDL_FillRect(editor->drawing_surface, NULL, 0);
}

void	editor_init(t_editor *editor)
{
	memset(editor, 0, sizeof(t_editor));
	ui_layout_load(&editor->layout, "layout.ui");
	editor->win_main = ui_list_get_window_by_id(editor->layout.windows, "win_main");
	editor->draw_button = ui_list_get_element_by_id(editor->layout.elements, "draw_button");
	editor->point_button = ui_list_get_element_by_id(editor->layout.elements, "point_button");
	editor->wall_button = ui_list_get_element_by_id(editor->layout.elements, "wall_button");
	editor->sector_button = ui_list_get_element_by_id(editor->layout.elements, "sector_button");
	editor->font = TTF_OpenFont("libs/libui/fonts/DroidSans.ttf", 20);
	ft_printf("[%s] %s\n", __FUNCTION__, SDL_GetError());
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
