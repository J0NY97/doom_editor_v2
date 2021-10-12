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
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
		editor->offset = vec2i(0, 0);

	// Moving
	if (editor->win_main->mouse_down == SDL_BUTTON_MIDDLE)
	{
		editor->offset.x -= (editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x);
		editor->offset.y -= (editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y);
	}

	// Draw
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_pos.y > 70 && editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
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
					wall->p1 = get_point_from_sector_around_radius(editor->selected_sector, editor->first_point, 0.0f);
					if (!wall->p1)
					{
						wall->p1 = ft_memalloc(sizeof(t_point));
						wall->p1->pos = editor->first_point;
					}
					wall->p2 = get_point_from_sector_around_radius(editor->selected_sector, editor->second_point, 0.0f);
					if (!wall->p2)
					{
						wall->p2 = ft_memalloc(sizeof(t_point));
						wall->p2->pos = editor->second_point;
					}
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

	if (editor->selected_sector)
	{
		// Point
		if (editor->point_button->state == UI_STATE_CLICK)
		{
			t_point	*point;
			if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
			{
				point = get_point_from_sector_around_radius(editor->selected_sector, actual_pos, 1.0f);
				if (point)
					editor->selected_point = point;
			}
			else if (editor->selected_point && editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
				editor->selected_point->pos = vec2i_add(editor->selected_point->pos, move_amount);
		}
		else
			editor->selected_point = NULL;
		// Wall
		if (editor->wall_button->state == UI_STATE_CLICK)
		{
			t_wall	*wall;
			if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
			{
				wall = get_wall_from_list_around_radius(editor->selected_sector->walls, actual_pos, 1.0f);
				if (wall)
					editor->selected_wall = wall;
			}
			else if (editor->selected_wall && editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
			{
				editor->selected_wall->p1->pos = vec2i_add(editor->selected_wall->p1->pos, move_amount);
				editor->selected_wall->p2->pos = vec2i_add(editor->selected_wall->p2->pos, move_amount);
			}
		}
		else
			editor->selected_wall = NULL;
	}
	else
	{
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}

	// Sector
	if (editor->sector_button->state == UI_STATE_CLICK)
	{
		t_sector	*sector;

		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
		{
			// We dont want to overwrite the currently selected sector with NULL if we dont find sector on mouseclick pos;
			sector = get_sector_from_list_around_radius(editor->sectors, actual_pos, 1);
			if (sector)
			{
				editor->selected_sector = sector;
				editor->selected_wall = NULL;
				editor->selected_point = NULL;
			}
		}
		else if (editor->selected_sector
			&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
			&& !editor->selected_point
			&& !editor->selected_wall)
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
	else if (editor->draw_button->state != UI_STATE_CLICK)
		editor->selected_sector = NULL;

	// Sector Edit Ok Button
	if (editor->sector_edit_ok_button->state == UI_STATE_CLICK)
	{
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}

	// Close Sector Menu Button
	if (editor->close_sector_edit_button->state == UI_STATE_CLICK)
	{
		editor->menu_sector_edit->show = 0;
		editor->selected_sector = NULL;
	}

	// Sector Edit Menu
	if (editor->sector_button->state == UI_STATE_CLICK && editor->selected_sector)
		editor->menu_sector_edit->show = 1;
	else
		editor->menu_sector_edit->show = 0;

	// Close Wall Menu Button
	if (editor->close_wall_edit_button->state == UI_STATE_CLICK)
	{
		editor->menu_wall_edit->show = 0;
		editor->selected_wall = NULL;
	}

	// Wall Edit Menu
	if (editor->wall_button->state == UI_STATE_CLICK && editor->selected_wall)
		editor->menu_wall_edit->show = 1;
	else
		editor->menu_wall_edit->show = 0;

	if (editor->split_wall_button->state == UI_STATE_CLICK && editor->selected_sector && editor->selected_wall)
	{
		t_point	*p1;
		t_point	*p2;
		t_point	*p3;
		t_wall	*new_wall;

		p1 = editor->selected_wall->p1;
		p2 = editor->selected_wall->p2;
		p3 = ft_memalloc(sizeof(t_point));
		p3->pos = get_wall_middle(editor->selected_wall);
		add_to_list(&editor->points, p3, sizeof(t_point));

		editor->selected_wall->p1 = p3;

		new_wall = ft_memalloc(sizeof(t_wall));
		new_wall->p1 = p1;
		new_wall->p2 = p3;
		add_to_list(&editor->selected_sector->walls, new_wall, sizeof(t_wall));
		add_to_list(&editor->walls, new_wall, sizeof(t_wall));
	}

	if (ui_input_exit(editor->floor_wall_angle_input))
	{
		int		angle;
		char	temp_str[10];
		angle = ft_clamp(ft_atoi(ui_input_text_get(editor->floor_wall_angle_input)), -45, 45);
		ft_b_itoa(angle, temp_str);
		ui_input_set_text(editor->floor_wall_angle_input, temp_str);
	}

	if (ui_input_exit(editor->ceiling_wall_angle_input))
	{
		int		angle;
		char	temp_str[10];
		angle = ft_clamp(ft_atoi(ui_input_text_get(editor->ceiling_wall_angle_input)), -45, 45);
		ft_b_itoa(angle, temp_str);
		ui_input_set_text(editor->ceiling_wall_angle_input, temp_str);
	}

	if (ui_input_exit(editor->wall_texture_scale_input))
	{
		float	scale;
		char	temp_str[10];
		scale = ft_fclamp(ft_atof(ui_input_text_get(editor->wall_texture_scale_input)), -10.0f, 10.0f);
		ft_b_ftoa(scale, 2, temp_str);
		ui_input_set_text(editor->wall_texture_scale_input, temp_str);
	}

	// Texture Menu
	if (editor->texture_menu_close_button->state == UI_STATE_CLICK)
		editor->texture_menu->show = 0;

	if (editor->floor_texture_button->state == UI_STATE_CLICK)
		editor->texture_menu->show = 1;
	else if (editor->ceiling_texture_button->state == UI_STATE_CLICK)
		editor->texture_menu->show = 1;
	else if (editor->wall_texture_button->state == UI_STATE_CLICK)
		editor->texture_menu->show = 1;
	else if (editor->portal_texture_button->state == UI_STATE_CLICK)
		editor->texture_menu->show = 1;

	//////////////////////
	// Save Window Events
	//////////////////////
	if (editor->save_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_save, UI_WINDOW_SHOW);// | UI_WINDOW_RAISE);

	if (editor->confirm_save_button->state == UI_STATE_CLICK)
	{
		int	len;

		len = ft_strlen(ui_input_text_get(editor->name_input));
		if (len < 1)
			ft_printf("[%s] Map name len is less than 1 (it\'s : %d), won\'t slide.\n", __FUNCTION__, len);
		else
		{
			char	*actual_full_path;

			actual_full_path = ft_strjoiner(MAP_PATH, ui_input_text_get(editor->name_input), ".dnd", NULL);
			set_map(editor, actual_full_path);
			ft_strdel(&actual_full_path);
		}
	}

	//////////////////////
	// Edit Window Events
	//////////////////////
	if (editor->edit_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_edit, UI_WINDOW_SHOW);// | UI_WINDOW_RAISE);
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
	if (editor->selected_sector)
		sector_render(editor, editor->selected_sector, 0xff00ff00);
	if (editor->selected_point)
		point_render(editor, editor->selected_point, 0xffffff00);
	else if (editor->selected_wall)
		wall_render(editor, editor->selected_wall, 0xffffff00);
}

void	user_render(t_editor *editor)
{
	draw_grid(editor->drawing_surface, editor->gap_size, editor->zoom);
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

	// Main Window
	editor->win_main = ui_list_get_window_by_id(editor->layout.windows, "win_main");
	editor->draw_button = ui_list_get_element_by_id(editor->layout.elements, "draw_button");
	editor->point_button = ui_list_get_element_by_id(editor->layout.elements, "point_button");
	editor->wall_button = ui_list_get_element_by_id(editor->layout.elements, "wall_button");
	editor->sector_button = ui_list_get_element_by_id(editor->layout.elements, "sector_button");
	editor->save_button = ui_list_get_element_by_id(editor->layout.elements, "save_button");
	editor->edit_button = ui_list_get_element_by_id(editor->layout.elements, "edit_button");

	editor->menu_sector_edit = ui_list_get_element_by_id(editor->layout.elements, "menu_sector_edit");
	editor->close_sector_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_sector_edit_button");
	editor->sector_edit_ok_button = ui_list_get_element_by_id(editor->layout.elements, "sector_edit_ok_button");
	editor->floor_texture_button = ui_list_get_element_by_id(editor->layout.elements, "floor_texture_button");
	editor->ceiling_texture_button = ui_list_get_element_by_id(editor->layout.elements, "ceiling_texture_button");

	editor->menu_wall_edit = ui_list_get_element_by_id(editor->layout.elements, "menu_wall_edit");
	editor->close_wall_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_wall_edit_button");
	editor->split_wall_button = ui_list_get_element_by_id(editor->layout.elements, "split_wall_button");
	editor->wall_texture_button = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_button");
	editor->portal_texture_button = ui_list_get_element_by_id(editor->layout.elements, "portal_texture_button");
	editor->floor_wall_angle_input = ui_list_get_element_by_id(editor->layout.elements, "floor_wall_angle_input");
	editor->ceiling_wall_angle_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_wall_angle_input");
	editor->wall_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_scale_input");

	editor->texture_menu = ui_list_get_element_by_id(editor->layout.elements, "texture_menu");
	editor->texture_menu->show = 0;
	editor->texture_menu_close_button = ui_list_get_element_by_id(editor->layout.elements, "texture_menu_close_button");

	// Save Window
	editor->win_save = ui_list_get_window_by_id(editor->layout.windows, "win_save");
	editor->endless_button = ui_list_get_element_by_id(editor->layout.elements, "endless_button");
	editor->story_button = ui_list_get_element_by_id(editor->layout.elements, "story_button");
	editor->name_input = ui_list_get_element_by_id(editor->layout.elements, "name_input");
	editor->confirm_save_button = ui_list_get_element_by_id(editor->layout.elements, "confirm_save_button");

	// Edit Window
	editor->win_edit = ui_list_get_window_by_id(editor->layout.windows, "win_edit");

	editor->font = TTF_OpenFont("libs/libui/fonts/DroidSans.ttf", 20);

	editor->map_name = ft_strdup("map_name.doom");
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

void	update_title_fps(SDL_Window *win, t_fps *fps)
{
	char	str[20];

	ft_b_itoa(fps->fps, str);
	SDL_SetWindowTitle(win, str);
}

void	fps_func(t_fps *fps)
{
	fps->curr = SDL_GetTicks();
	fps->count++;
	if (fps->curr - fps->prev >= 1000)
	{
		fps->prev = fps->curr;
		fps->fps = fps->count;
		fps->count = 0;
	}
}

int	main(int ac, char **av)
{
	t_editor	editor;
	int			run;
	SDL_Event	e;

	ui_sdl_init();
	editor_init(&editor);
	draw_init(&editor);
	if (args_parser(&editor, ac, av))
		try_reading_map(&editor);
	else
		ft_printf("[%s] No map given to open.\n", __FUNCTION__);

	t_fps	fps;
	memset(&fps, 0, sizeof(t_fps));
	run = 1;
	while (run)
	{
		fps_func(&fps);
		update_title_fps(editor.win_main->win, &fps);
		if (editor.win_main->wants_to_close)
			run = 0;
		while (SDL_PollEvent(&e))
		{
			ui_layout_event(&editor.layout, e);

			// User Events
			user_events(&editor, e);

			if (e.key.keysym.scancode == SDL_SCANCODE_P)
			{
				ui_element_print(editor.point_button);
				ui_element_print(editor.wall_button);
			}
		}
		// User Render
		user_render(&editor);

		ui_layout_render(&editor.layout);
	}
}
