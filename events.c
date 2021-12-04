#include "editor.h"

/*
 * This calculates the actual position in game for the mouse;
*/
void	calculate_hover(t_editor *editor)
{
	editor->last_mouse_pos = editor->mouse_pos;
	editor->mouse_pos.x = (editor->win_main->mouse_pos.x
			/ (editor->gap_size * editor->zoom)) + editor->offset.x;
	editor->mouse_pos.y = (editor->win_main->mouse_pos.y
			/ (editor->gap_size * editor->zoom)) + editor->offset.y;
	editor->mouse_pos_screen.x = (editor->mouse_pos.x - editor->offset.x)
		* (editor->gap_size * editor->zoom);
	editor->mouse_pos_screen.y = (editor->mouse_pos.y - editor->offset.y)
		* (editor->gap_size * editor->zoom);
	editor->move_amount.x = editor->mouse_pos.x - editor->last_mouse_pos.x;
	editor->move_amount.y = editor->mouse_pos.y - editor->last_mouse_pos.y;
	if (editor->win_main->scroll)
	{
		editor->zoom
			= ft_fclamp(editor->zoom + editor->win_main->scroll / 10.0f,
				0.1f, 10.0f);
		editor->update_grid = 1;
	}
}

void	update_info_label(t_editor *editor)
{
	t_rgba	new_col;

	if (editor->info_label->show
		&& SDL_GetTicks() - editor->info_label_start_timer
		>= editor->info_label_timer)
	{
		new_col = hex_to_rgba(ui_label_get_color(editor->info_label));
		new_col.a -= 2;
		ui_label_color_set(editor->info_label, rgba_to_hex(new_col));
		if (new_col.a <= 0)
			editor->info_label->show = 0;
	}
}

void	grid_events(t_editor *editor, SDL_Event e)
{
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
		editor->offset = vec2i(0, 0);
	if (editor->win_main->mouse_down == SDL_BUTTON_MIDDLE)
	{
		editor->offset.x -= (editor->win_main->mouse_pos.x
				- editor->win_main->mouse_pos_prev.x);
		editor->offset.y -= (editor->win_main->mouse_pos.y
				- editor->win_main->mouse_pos_prev.y);
	}
	if (editor->draw_button->was_click)
	{
		editor->selected_sector = NULL;
		editor->selected_entity = NULL;
		editor->selected_event = NULL;
		editor->selected_sprite = NULL;
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}
}

/*
 * 1. Remove all neighbors from walls that sector doesnt exist anymore;
*/
void	wall_cleanup(t_editor *editor)
{
	t_wall	*wall;
	t_list	*curr;

	curr = editor->walls;
	while (curr)
	{
		wall = curr->content;
		if (!get_sector_with_id(editor->sectors, wall->neighbor_id))
			wall->neighbor = NULL;
		curr = curr->next;
	}
}

/*
 * 1. Remove all the points not a part of a sector;
 * 2. Remove all walls with either points NULL;
 * 2.5. Remove all neighbors not connected;
 * 		(if a wall has neighbor but it's corresponding neighbor/sector doesn't
 * 		have same wall as neighbor, or at all);
 * 3. Remove all sectors with no walls;
*/
void	sector_cleanup(t_editor *editor)
{
	remove_all_lonely_walls(editor);
	ft_printf("[%s] Done cleaning walls!\n", __FUNCTION__);
	remove_all_lonely_points(editor);
	ft_printf("[%s] Done cleaning points!\n", __FUNCTION__);
	wall_cleanup(editor);
	ft_printf("[%s] Cleanup crew done!\n", __FUNCTION__);
}

void	remove_button_events(t_editor *editor)
{
	int	was_removed;

	was_removed = 0;
	if (editor->remove_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_sector)
		{
			was_removed = remove_sector(editor, editor->selected_sector);
			editor->selected_sector = NULL;
		}
		else if (editor->selected_entity)
			was_removed = remove_entity(editor, editor->selected_entity);
		if (was_removed)
		{
			editor->selected_point = NULL;
			editor->selected_wall = NULL;
			editor->selected_sprite = NULL;
			editor->selected_entity = NULL;
			sector_cleanup(editor);
		}
	}
}

/*
 * NOTE: We dont want to overwrite the currently selected sector with NULL,
 * 		if we dont find sector on mouseclick pos;
*/
void	select_sector(t_editor *editor)
{
	t_sector	*sector;

	sector = get_sector_from_list_around_radius(
			editor->sectors, editor->mouse_pos, 1);
	if (sector)
	{
		editor->selected_sector = sector;
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
		editor->active_texture_button = NULL;
		set_sector_ui(editor, editor->selected_sector);
	}
}

void	attempt_adding_wall_to_sector(t_editor *editor, t_sector *sector)
{
	if (editor->first_point_set && editor->second_point_set
		&& compare_veci(editor->first_point.v, editor->second_point.v, 2))
		editor->second_point_set = 0;
	if (editor->first_point_set && editor->second_point_set)
	{
		add_wall_to_sector_at_pos(editor, editor->selected_sector,
			editor->first_point, editor->second_point);
		editor->first_point_set = 0;
		editor->second_point_set = 0;
		if (!editor->selected_sector->first_point_set)
		{
			editor->selected_sector->first_point = editor->first_point;
			editor->selected_sector->first_point_set = 1;
		}
		if (compare_veci(editor->second_point.v,
				editor->selected_sector->first_point.v, 2))
			editor->selected_sector = NULL;
		else
		{
			editor->first_point = editor->second_point;
			editor->first_point_set = 1;
		}
	}
}

/*
 * If we dont have selected_sector, we create one;
 * If we have selected_sector, we add walls to it;
*/
void	sector_draw_events(t_editor *editor)
{
	if (editor->draw_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
	{
		if (!editor->selected_sector)
			editor->selected_sector = add_sector(editor);
		if (editor->selected_sector)
		{
			if (!editor->first_point_set)
			{
				editor->first_point_set = 1;
				editor->first_point = editor->mouse_pos;
			}
			else if (!editor->second_point_set)
			{
				editor->second_point_set = 1;
				editor->second_point = editor->mouse_pos;
			}
			attempt_adding_wall_to_sector(editor, editor->selected_sector);
		}
	}
}

/*
 * NOTE: MOVE SECTOR only if nothing else is selected (point/wall);
*/
void	sector_edit_events(t_editor *editor)
{
	if (editor->select_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
		select_sector(editor);
	else if (editor->selected_sector
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !editor->selected_point && !editor->selected_wall)
		move_sector(editor->selected_sector, editor->move_amount);
	if (editor->selected_sector && editor->sector_edit_menu->show)
		get_sector_ui(editor, editor->selected_sector);
	if (editor->sector_edit_ok_button->state == UI_STATE_CLICK)
	{
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}
	if (editor->close_sector_edit_button->state == UI_STATE_CLICK)
	{
		editor->sector_edit_menu->show = 0;
		editor->selected_sector = NULL;
		editor->selected_wall = NULL;
		editor->selected_sprite = NULL;
		editor->selected_point = NULL;
	}
}

void	sector_events(t_editor *editor)
{
	if (editor->sector_button->state == UI_STATE_CLICK)
	{
		if (editor->select_button->state == UI_STATE_CLICK)
		{
			sector_edit_events(editor);
			editor->sector_edit_menu->show = 0;
			if (editor->selected_sector)
				editor->sector_edit_menu->show = 1;
		}
		else
		{
			sector_draw_events(editor);
			editor->sector_edit_menu->show = 0;
		}
	}
	else
	{
		editor->selected_sector = NULL;
		editor->selected_wall = NULL;
		editor->selected_sprite = NULL;
		editor->selected_point = NULL;
		editor->sector_edit_menu->show = 0;
		editor->wall_button->state = UI_STATE_DEFAULT;
		editor->point_button->state = UI_STATE_DEFAULT;
	}
}

void	draw_wall_on_surface(SDL_Surface *surface, t_wall *wall, float scale)
{
	int		amount_x;
	int		amount_y;
	int		ii;
	int		jj;

	amount_x = ceil(wall->width / wall->texture_scale);
	amount_y = ceil(wall->height / wall->texture_scale);
	jj = -1;
	while (++jj <= amount_y)
	{
		ii = -1;
		while (++ii <= amount_x)
			SDL_BlitScaled(wall->texture, NULL, surface,
				&(SDL_Rect){ii * scale, jj * scale, scale, scale});
	}
}

/*
 * SDL_Surface	*texture; // DONT FREE;
 * int size; this is a unit of measurement; (...no s**t (hard to explain));
*/
void	draw_sprites_on_surface(
		t_editor *editor, SDL_Surface *surface, t_list *sprites, float aspect)
{
	t_sprite	*sprite;
	t_vec4i		xywh;
	SDL_Surface	*texture;
	int			size;

	size = 64;
	while (sprites)
	{
		sprite = sprites->content;
		texture = editor->wall_textures[sprite->texture_id];
		if (sprite->type == STATIC)
			xywh = vec4i(0, 0, texture->w, texture->h);
		else
			xywh = vec4i(0, 0, 64, 64);
		sprite->pos.w = (size * sprite->scale) * aspect;
		sprite->pos.h = (float)xywh.h * ((float)sprite->pos.w / (float)xywh.w);
		SDL_BlitScaled(texture, &(SDL_Rect){xywh.x, xywh.y, xywh.w, xywh.h},
			surface, &(SDL_Rect){sprite->pos.x, sprite->pos.y,
			sprite->pos.w, sprite->pos.h});
		sprites = sprites->next;
	}
}

void	draw_selected_sprite(t_editor *editor, SDL_Surface *surface)
{
	t_vec2i	p1;
	t_vec2i	p2;

	if (editor->selected_sprite)
	{
		p1 = vec2i(editor->selected_sprite->pos.x,
				editor->selected_sprite->pos.y);
		p2 = vec2i(p1.x + editor->selected_sprite->pos.w,
				p1.y + editor->selected_sprite->pos.h);
		ui_surface_rect_draw(surface, p1, p2, 0xff0000ff);
	}
}

void	visualize_wall(
		t_editor *editor, t_wall *wall)
{
	SDL_Surface	*surface;
	float		aspect;
	float		scale;
	int			size;

	SDL_SetRenderTarget(editor->win_main->renderer,
		editor->wall_render->texture);
	SDL_RenderClear(editor->win_main->renderer);
	size = 64;
	aspect = get_ratio_f(vec2(wall->width * size, wall->height * size),
			vec2(editor->wall_render->pos.w, editor->wall_render->pos.h));
	scale = (size * wall->texture_scale) * aspect;
	surface = ui_surface_new(wall->width * size * aspect,
			wall->height * size * aspect);
	wall->texture = editor->wall_textures[wall->wall_texture];
	draw_wall_on_surface(surface, wall, scale);
	draw_sprites_on_surface(editor, surface, wall->sprites, aspect);
	draw_selected_sprite(editor, surface);
	ui_surface_draw_border(surface, 1, 0xff00ff00);
	SDL_UpdateTexture(editor->wall_render->texture, &(SDL_Rect){0, 0,
		ft_min(surface->w, editor->wall_render->current_texture_size.x),
		ft_min(surface->h, editor->wall_render->current_texture_size.y)},
		surface->pixels, surface->pitch);
	SDL_FreeSurface(surface);
}

void	select_sprite(t_editor *editor)
{
	t_vec2i		actual_mouse_pos;
	t_sprite	*sprite;

	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& ui_element_is_hover(editor->wall_render))
	{
		veci_sub(actual_mouse_pos.v, editor->win_main->mouse_pos.v,
			editor->wall_render->screen_pos.v, 2);
		sprite = get_sprite_from_list_at_pos(editor->selected_wall->sprites,
				actual_mouse_pos);
		if (sprite)
		{
			editor->selected_sprite = sprite;
			set_sprite_ui(editor, editor->selected_sprite);
		}
	}
}

void	edit_sprite(t_editor *editor)
{
	t_vec2i	mouse_diff;
	char	temp_str[20];

	if (!editor->selected_sprite)
		return ;
	veci_sub(mouse_diff.v,
		editor->win_main->mouse_pos.v, editor->win_main->mouse_pos_prev.v, 2);
	if (editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& ui_element_is_hover(editor->sprite_edit_menu))
	{
		editor->selected_sprite->pos.x += mouse_diff.x;
		editor->selected_sprite->pos.y += mouse_diff.y;
		ui_input_set_text(editor->sprite_x_input,
			ft_b_itoa(editor->selected_sprite->pos.x, temp_str));
		ui_input_set_text(editor->sprite_y_input,
			ft_b_itoa(editor->selected_sprite->pos.y, temp_str));
	}
	get_sprite_ui(editor, editor->selected_sprite);
}

void	sprite_events(t_editor *editor)
{
	if (!editor->selected_wall || !editor->selected_sector)
		return ;
	if (editor->sprite_add_button->state == UI_STATE_CLICK)
	{
		if (!editor->selected_sprite)
			editor->selected_sprite
				= add_sprite_to_wall(editor, editor->selected_wall);
		else
			editor->selected_sprite = NULL;
	}
	else if (editor->sprite_remove_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_sprite)
		{
			remove_sprite(editor, editor->selected_sprite);
			editor->selected_sprite = NULL;
			--editor->selected_wall->sprite_amount;
			ft_printf("Sprite Removed (total : %d)\n",
				editor->selected_wall->sprite_amount);
		}
	}
	select_sprite(editor);
	edit_sprite(editor);
}

/*
 * NOTE: we dont want to make selected_wall NULL if we dont select a wall;
*/
void	select_wall(t_editor *editor)
{
	t_wall	*wall;

	wall = get_wall_from_list_around_radius(
			editor->selected_sector->walls, editor->mouse_pos, 1.0f);
	if (wall)
	{
		editor->selected_wall = wall;
		editor->active_texture_button = NULL;
		editor->selected_sprite = NULL;
		set_wall_ui(editor, wall);
	}
}

/*
 * NOTE: only move wall if we are NOT hovering over the sprite_edit_menu;
*/
void	wall_select_events(t_editor *editor)
{
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor)
		&& editor->selected_sector)
		select_wall(editor);
	else if (editor->selected_wall
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !(editor->wall_render->show == 1
			&& ui_element_is_hover(editor->sprite_edit_menu)))
		move_wall(editor->selected_wall, editor->move_amount);
	if (editor->close_wall_edit_button->state == UI_STATE_CLICK)
	{
		editor->menu_wall_edit->show = 0;
		editor->selected_wall = NULL;
		editor->selected_sprite = NULL;
	}
	if (editor->selected_wall)
	{
		if (editor->split_wall_button->state == UI_STATE_CLICK)
			split_wall(editor, editor->selected_sector, editor->selected_wall);
		get_wall_ui(editor, editor->selected_wall);
	}
}

void	wall_events(t_editor *editor)
{
	if (editor->sector_button->state == UI_STATE_CLICK
		&& editor->wall_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_wall)
		{
			editor->menu_wall_edit->show = 1;
			editor->sprite_edit_menu->show = 1;
			sprite_events(editor);
			visualize_wall(editor, editor->selected_wall);
		}
		else
		{
			editor->selected_sprite = NULL;
			editor->menu_wall_edit->show = 0;
			editor->sprite_edit_menu->show = 0;
		}
		wall_select_events(editor);
	}
	else
	{
		editor->selected_wall = NULL;
		editor->menu_wall_edit->show = 0;
		editor->selected_sprite = NULL;
		editor->sprite_edit_menu->show = 0;
	}
}

void	point_select_events(t_editor *editor)
{
	t_point	*point;

	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
	{
		point = get_point_from_sector_around_radius(
				editor->selected_sector, editor->mouse_pos, 1.0f);
		if (point)
			editor->selected_point = point;
	}
	else if (editor->selected_point
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		editor->selected_point->pos
			= vec2i_add(editor->selected_point->pos, editor->move_amount);
}

void	point_events(t_editor *editor)
{
	if (editor->sector_button->state == UI_STATE_CLICK
		&& editor->point_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_sector)
		{
			if (editor->point_button->state == UI_STATE_CLICK)
				point_select_events(editor);
			else
				editor->selected_point = NULL;
		}
		else
			editor->selected_point = NULL;
	}
	else
		editor->selected_point = NULL;
}

t_texture_elem	*get_active_texture_elem(t_editor *editor)
{
	t_texture_elem	*new_texture_elem;
	t_list			*curr;

	if (ui_list_radio_event(editor->texture_buttons,
			&editor->active_texture_button))
	{
		curr = editor->texture_elems;
		while (curr)
		{
			new_texture_elem = curr->content;
			if (new_texture_elem->button == editor->active_texture_button)
			{
				editor->active_texture_button_id = new_texture_elem->id;
				return (new_texture_elem);
			}
			curr = curr->next;
		}
	}
	return (NULL);
}

void	texture_menu_select_events(t_editor *editor)
{
	t_list				*curr;
	t_texture_elem		*selected_texture_elem;
	t_texture_something	*t_something;

	selected_texture_elem = get_active_texture_elem(editor);
	if (selected_texture_elem
		&& editor->active_texture_button_id > -1)
	{
		t_something = NULL;
		curr = editor->texture_somethings;
		while (curr)
		{
			t_something = curr->content;
			if (t_something->button == editor->active_texture_opening_button)
			{
				t_something->id = selected_texture_elem->id;
				ui_element_image_set(t_something->image, UI_STATE_DEFAULT,
					editor->wall_textures[selected_texture_elem->id]);
				break ;
			}
			curr = curr->next;
		}
	}
}

void	texture_menu_events(t_editor *editor)
{
	if (ui_list_radio_event(editor->texture_opening_buttons,
			&editor->active_texture_opening_button))
	{
		ui_label_set_text(editor->texture_menu_label,
			ui_button_get_text(editor->active_texture_opening_button));
		editor->active_texture_button = NULL;
		editor->active_texture_button_id = -1;
		editor->texture_menu->show = 1;
	}
	if (!editor->texture_menu->show
		|| !editor->active_texture_opening_button
		|| editor->texture_menu_close_button->state == UI_STATE_CLICK)
	{
		editor->texture_menu->show = 0;
		if (editor->active_texture_opening_button)
			editor->active_texture_opening_button->state = UI_STATE_DEFAULT;
		editor->active_texture_opening_button = NULL;
		editor->active_texture_button_id = -1;
		return ;
	}
	texture_menu_select_events(editor);
}

/*
 * NOTE: we dont want to overwrite the currently selected entity
 * 		if we dont actually find a new one;
*/
void	select_entity(t_editor *editor)
{
	t_entity	*entity;

	entity = get_entity_from_list_around_radius(
			editor->entities, editor->mouse_pos, 1.0f);
	if (entity)
	{
		editor->selected_entity = entity;
		set_entity_ui(editor, editor->selected_entity);
	}
}

void	entity_draw_events(t_editor *editor)
{
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
		add_entity(editor)->pos = editor->mouse_pos;
}

/*
 * NOTE:
 *		If the dropdown menu is open, lets ignore all other inputs;
*/
void	entity_select_events(t_editor *editor)
{
	editor->entity_yaw_input->event = 1;
	editor->entity_yaw_slider->event = 1;
	editor->entity_z_input->event = 1;
	if (ui_dropdown_is_open(editor->entity_dropdown))
	{
		editor->entity_yaw_input->event = 0;
		editor->entity_yaw_slider->event = 0;
		editor->entity_z_input->event = 0;
	}
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !ui_element_is_hover(editor->entity_edit_menu))
		select_entity(editor);
	else if (editor->selected_entity
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		editor->selected_entity->pos = vec2i_add(
				editor->selected_entity->pos, editor->move_amount);
	if (editor->selected_entity)
		get_entity_ui(editor, editor->selected_entity);
	if (!editor->selected_entity
		|| editor->close_entity_edit_button->state == UI_STATE_CLICK)
	{
		editor->entity_edit_menu->show = 0;
		editor->selected_entity = NULL;
	}
}

void	entity_events(t_editor *editor)
{
	if (editor->entity_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_entity)
			editor->entity_edit_menu->show = 1;
		else
			editor->entity_edit_menu->show = 0;
		if (editor->draw_button->state == UI_STATE_CLICK)
			entity_draw_events(editor);
		else if (editor->select_button->state == UI_STATE_CLICK)
			entity_select_events(editor);
	}
	else
	{
		editor->selected_entity = NULL;
		editor->entity_edit_menu->show = 0;
	}
}

void	event_removing_events(t_editor *editor)
{
	if (editor->remove_event_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_event && editor->selected_event_elem)
		{
			remove_event(editor, editor->selected_event);
			editor->selected_event_elem = NULL;
			editor->active_event_elem = NULL;
			editor->selected_event = NULL;
		}
	}
}

/*
 * if we have selected event elem,
 * 		we update it,
 * 	otherwise we make new event elem;
*/
void	event_adding_events(t_editor *editor)
{
	if (editor->add_event_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_event_elem == NULL)
		{
			editor->selected_event = add_event(editor);
			editor->selected_event_elem = editor->selected_event->elem;
			editor->active_event_elem = editor->selected_event->elem->button;
		}
		update_event(editor, editor->selected_event_elem->event);
		update_event_elem(editor->selected_event_elem);
		editor->selected_event_elem = NULL;
		editor->active_event_elem = NULL;
		editor->selected_event = NULL;
	}
}

/*
 * Update ID Dropdown;
 *	when id dropdown opens we add buttons for each sprite/sector id,
 *	depending on which action type is selected;
*/
void	event_id_dropdown_update(t_editor *editor)
{
	char		**texts;
	t_ui_recipe	*recipe;

	if (editor->event_id_dropdown->show
		&& ui_dropdown_open(editor->event_id_dropdown))
	{
		texts = NULL;
		if (ui_dropdown_active(editor->event_action_dropdown)
			== editor->event_action_sector_button)
			texts = gen_sector_id_texts(editor->sectors);
		else
			texts = gen_sprite_id_texts(editor->sprites);
		if (!texts)
			return ;
		recipe = ui_list_get_recipe_by_id(
				editor->layout.recipes, "event_id_button");
		create_buttons_to_list_from_texts_remove_extra(
			ui_dropdown_get_menu_element(editor->event_id_dropdown),
			texts, recipe);
		ft_arraydel(texts);
	}
}

void	event_select(t_editor *editor)
{
	t_list	*curr;

	if (ui_list_radio_event(editor->event_element_buttons,
			&editor->active_event_elem))
	{
		curr = editor->event_elements;
		while (curr)
		{
			if (((t_event_elem *)curr->content)->button
				== editor->active_event_elem)
			{
				editor->selected_event_elem = curr->content;
				editor->selected_event = editor->selected_event_elem->event;
				break ;
			}
			curr = curr->next;
		}
		if (editor->selected_event)
			set_event_ui(editor, editor->selected_event);
	}
}

/*
 * NOTE:
 * 		Ignore all ui inputs if we have dropdowns open;
*/
void	event_select_events(t_editor *editor)
{
	ui_menu_get_menu(editor->event_menu)->event_children = 1;
	editor->event_scrollbar->event = 1;
	editor->event_sector_input->event = 1;
	editor->event_min_input->event = 1;
	editor->event_max_input->event = 1;
	editor->event_speed_input->event = 1;
	if (ui_dropdown_is_open(editor->event_action_dropdown)
		|| ui_dropdown_is_open(editor->event_type_dropdown)
		|| ui_dropdown_is_open(editor->event_id_dropdown))
	{
		ui_menu_get_menu(editor->event_menu)->event_children = 0;
		editor->event_scrollbar->event = 0;
		editor->event_sector_input->event = 0;
		editor->event_min_input->event = 0;
		editor->event_max_input->event = 0;
		editor->event_speed_input->event = 0;
	}
	event_select(editor);
	event_adding_events(editor);
	event_removing_events(editor);
	event_id_dropdown_update(editor);
}

void	event_events(t_editor *editor)
{
	if (editor->event_button->state == UI_STATE_CLICK)
	{
		editor->event_edit_menu->show = 1;
		event_select_events(editor);
	}
	else
	{
		editor->selected_event = NULL;
		editor->event_edit_menu->show = 0;
	}
}

void	spawn_events(t_editor *editor)
{
	char	temp_str[20];

	if (editor->spawn_button->state == UI_STATE_CLICK)
		editor->spawn_edit_menu->show = 1;
	else
		editor->spawn_edit_menu->show = 0;
	if (!editor->spawn_edit_menu->show)
		return ;
	if (editor->spawn_button->was_click)
		ui_input_set_text(editor->spawn_yaw_input,
			ft_b_itoa(editor->spawn.yaw, temp_str));
	if (editor->draw_button->state == UI_STATE_CLICK)
		if (!hover_over_open_menus(editor))
			if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
				editor->spawn.pos = editor->mouse_pos;
	if (editor->select_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		editor->spawn.pos = vec2i_add(editor->spawn.pos, editor->move_amount);
	if (ui_input_exit(editor->spawn_yaw_input))
	{
		editor->spawn.yaw = ft_clamp(
				ft_atoi(ui_input_get_text(editor->spawn_yaw_input)), 0, 360);
		ui_input_set_text(editor->spawn_yaw_input,
			ft_b_itoa(editor->spawn.yaw, temp_str));
	}
}

void	update_amount_info(t_editor *editor)
{
	char	*final;

	final = ft_sprintf("points : %d\nwalls : %d\nsectors : %d\n"
			"entities : %d\nsprites : %d\nevents : %d\n",
			editor->point_amount, editor->wall_amount, editor->sector_amount,
			editor->entity_amount, editor->sprite_amount, editor->event_amount);
	ui_label_set_text(editor->misc_info_label, final);
	ft_strdel(&final);
}

int	update_sector_info(t_editor *editor, t_sector *sector)
{
	char		*final;

	if (sector)
	{
		final = ft_sprintf("iD : %d\npos : %d, %d\nWalls : %d\n",
				sector->id, sector->center.x, sector->center.y,
				sector->wall_amount);
		ui_label_set_text(editor->sector_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sector_info_label, "NONE");
	return (0);
}

int	update_point_info(t_editor *editor, t_point *point)
{
	char	*final;

	if (point)
	{
		final = ft_sprintf("iD : %d\npos : %d, %d\n",
				point->id, point->pos.x, point->pos.y);
		ui_label_set_text(editor->sub_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sub_info_label, "NONE");
	return (0);
}

int	update_sprite_info(t_editor *editor, t_sprite *sprite)
{
	char		*final;

	if (sprite)
	{
		sprite = editor->selected_sprite;
		final = ft_sprintf("iD : %d\npos : %d, %d\n",
				sprite->id, sprite->pos.x, sprite->pos.y);
		ui_label_set_text(editor->sprite_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sprite_info_label, "NONE");
	return (0);
}

int	update_wall_info(t_editor *editor, t_wall *wall)
{
	char	*final;

	if (wall)
	{
		final = ft_sprintf("iD : %d\np1 : %d, %d\np2 : %d, %d\ndist : %.2f\n",
				wall->id, wall->p1->pos.x, wall->p1->pos.y, wall->p2->pos.x,
				wall->p2->pos.y, distance(wall->p1->pos.v, wall->p2->pos.v, 2));
		ui_label_set_text(editor->sub_info_label, final);
		ft_strdel(&final);
		return (1);
	}
	else
		ui_label_set_text(editor->sub_info_label, "NONE");
	return (0);
}

/*
 * NOTE:
 *		Only one of point or walls should be changing the sub info text;
*/
void	info_menu_events(t_editor *editor, SDL_Event e)
{
	char	*final_str;

	if (e.type == SDL_MOUSEMOTION)
	{
		final_str = ft_sprintf("%d, %d",
				editor->mouse_pos.x, editor->mouse_pos.y);
		ui_label_set_text(editor->mouse_info_label, final_str);
		ft_strdel(&final_str);
	}
	update_amount_info(editor);
	update_sector_info(editor, editor->selected_sector);
	if (!update_point_info(editor, editor->selected_point))
		update_wall_info(editor, editor->selected_wall);
	update_sprite_info(editor, editor->selected_sprite);
}

void	sector_hover_info_events(t_editor *editor)
{
	char		*temp;
	t_sector	*sector;

	if (!editor->hovered_sector)
	{
		editor->sector_hover_info_menu->show = 0;
		return ;
	}
	sector = editor->hovered_sector;
	editor->sector_hover_info_menu->show = 1;
	temp = ft_sprintf("Sector Info\n\nID: %d\n%27cFloor-%7cCeiling-\n"
			"Height : %14d %16d\nTexture : %12d %16d\n"
			"Texture Scale : %2.2f %12.2f\n",
			sector->id, ' ', ' ', sector->floor_height, sector->ceiling_height,
			sector->floor_texture, sector->ceiling_texture,
			sector->floor_scale, sector->ceiling_scale);
	ui_label_set_text(editor->sector_hover_info_label, temp);
	ft_strdel(&temp);
	ui_element_pos_set2(editor->sector_hover_info_menu,
		vec2(editor->win_main->mouse_pos.x + 10,
			editor->win_main->mouse_pos.y + 10));
}

void	save_button_events(t_editor *editor)
{
	char	*actual_full_path;

	if (ft_strlen(ui_input_get_text(editor->name_input)) < 1)
		ft_printf("[%s] Map name len is less than 1.\n", __FUNCTION__);
	else
	{
		if (editor->map_type == MAP_TYPE_STORY)
			actual_full_path = ft_strjoiner(MAP_PATH,
					ui_input_get_text(editor->name_input), ".dnds", NULL);
		else
			actual_full_path = ft_strjoiner(MAP_PATH,
					ui_input_get_text(editor->name_input), ".dnde", NULL);
		set_map(editor, actual_full_path);
		ft_strdel(&actual_full_path);
		ui_window_flag_set(editor->win_save, UI_WINDOW_HIDE);
		send_info_message(editor, "Map Saved Successfully!");
	}
}

void	save_window_events(t_editor *editor)
{
	if (editor->save_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_save, UI_WINDOW_SHOW | UI_WINDOW_RAISE);
	if (!editor->win_save->show)
		return ;
	if (editor->endless_checkbox->was_click)
		editor->map_type = MAP_TYPE_ENDLESS;
	else if (editor->story_checkbox->was_click)
		editor->map_type = MAP_TYPE_STORY;
	ui_checkbox_toggle_accordingly(editor->endless_checkbox,
		editor->map_type == MAP_TYPE_ENDLESS);
	ui_checkbox_toggle_accordingly(editor->story_checkbox,
		editor->map_type == MAP_TYPE_STORY);
	if (ui_button(editor->confirm_save_button))
		save_button_events(editor);
}

void	edit_window_events(t_editor *editor)
{
	if (editor->edit_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_edit, UI_WINDOW_SHOW | UI_WINDOW_RAISE);
	if (!editor->win_edit->show)
		return ;
	if (ui_input_exit(editor->map_scale_input))
		editor->map_scale = ft_atof(ui_input_get_text(editor->map_scale_input));
}

void	user_events(t_editor *editor, SDL_Event e)
{
	calculate_hover(editor);
	update_info_label(editor);
	grid_events(editor, e);
	remove_button_events(editor);
	sector_events(editor);
	wall_events(editor);
	point_events(editor);
	texture_menu_events(editor);
	entity_events(editor);
	event_events(editor);
	spawn_events(editor);
	info_menu_events(editor, e);
	sector_hover_info_events(editor);
	save_window_events(editor);
	edit_window_events(editor);
}
