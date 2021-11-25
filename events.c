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
	// Reset Grid Pos
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
		editor->offset = vec2i(0, 0);

	// Moving Grid
	if (editor->win_main->mouse_down == SDL_BUTTON_MIDDLE)
	{
		editor->offset.x -= (editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x);
		editor->offset.y -= (editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y);
	}

	// Removing all the selected whatevers.
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
	if (editor->remove_button->state == UI_STATE_CLICK)
	{
		int	was_removed = 0;
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

void	select_sector(t_editor *editor)
{
	t_sector	*sector;

	// We dont want to overwrite the currently selected sector with NULL if we dont find sector on mouseclick pos;
	sector = get_sector_from_list_around_radius(
			editor->sectors, editor->mouse_pos, 1);
	if (sector) // if new sector selected;
	{
		editor->selected_sector = sector;
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
		editor->active_texture_button = NULL; // unselect currently selected texture;
		set_sector_ui(editor, editor->selected_sector);
	}
}

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
			// We dont want you to be able to draw a 0 length wall;
			if (editor->first_point_set
				&& compare_veci(editor->first_point.v, editor->second_point.v, 2))
				editor->second_point_set = 0;
			if (editor->first_point_set && editor->second_point_set)
			{
				// NOTE: add_wall_to_sector_at_pos, doesnt check if we have anything in the 2 points given;
				add_wall_to_sector_at_pos(editor, editor->selected_sector,
					editor->first_point, editor->second_point);
				editor->first_point_set = 0;
				editor->second_point_set = 0;
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

void	sector_edit_events(t_editor *editor)
{
	// Select sector;
	if (editor->select_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
		select_sector(editor);
	else if (editor->selected_sector // MOVE SECTOR ( only if nothing else is selected (point/wall) )
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !editor->selected_point && !editor->selected_wall)
		move_sector(editor->selected_sector, editor->move_amount);
	// Update sector from ui;
	if (editor->selected_sector && editor->sector_edit_menu->show)
		get_sector_ui(editor, editor->selected_sector);
	// Sector Edit Ok Button // unselect wall / point;
	if (editor->sector_edit_ok_button->state == UI_STATE_CLICK)
	{
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}
	// Close Sector Menu Button
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

void	render_wall_on_sprite_menu(
		t_editor *editor, t_sector *sector, t_wall *wall)
{
	// Clear the wall render from last frame, since we dont draw over the whole texture;
	ui_texture_fill(editor->win_main->renderer,
		editor->wall_render->texture, 0xff000000);

	SDL_Surface	*surface;
	t_vec2		dist;
	float		aspect;
	int			size;
	float		amount_x;
	float		amount_y;

	dist.x = distance(wall->p1->pos.v, wall->p2->pos.v, 2);
	dist.y = (sector->ceiling_height - sector->floor_height);
	size = 64;
	amount_x = dist.x / wall->texture_scale;
	amount_y = dist.y / wall->texture_scale;
	aspect = get_ratio_f(vec2(dist.x * size, dist.y * size),
			vec2(editor->wall_render->pos.w, editor->wall_render->pos.h));

	float scale = (size * wall->texture_scale) * aspect;
	surface = ui_surface_new(dist.x * size * aspect, dist.y * size * aspect);

	SDL_Surface	*texture = editor->wall_textures[wall->wall_texture]; // DONT FREE!

	for (int j = 0; j <= amount_y; j++)
		for (int i = 0; i <= amount_x; i++)
			SDL_BlitScaled(texture, NULL, surface, &(SDL_Rect){i * scale, j * scale, scale, scale});

	// Render the sprites;
	t_sprite	*sprite;
	t_list		*curr;
	t_vec4i		xywh;

	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		texture = editor->wall_textures[sprite->texture];
		xywh = vec4i(0, 0, texture->w, texture->h);
		if (sprite->type == STATIC)
		{
			sprite->pos.w = (size * sprite->scale) * aspect;
			sprite->pos.h = (float)texture->h * ((float)sprite->pos.w / (float)texture->w);
		}
		else
		{
			xywh.w = 64;
			xywh.h = 64;
			sprite->pos.w = (size * sprite->scale) * aspect;
			sprite->pos.h = (float)texture->h * ((float)sprite->pos.w / (float)xywh.w);
		}
		SDL_BlitScaled(texture, &(SDL_Rect){xywh.x, xywh.y, xywh.w, xywh.h},
			surface, &(SDL_Rect){sprite->pos.x, sprite->pos.y, sprite->pos.w, sprite->pos.h});
		curr = curr->next;
	}
	// Draw rect around selected sprite;
	t_vec2i	p1;
	t_vec2i	p2;
	if (editor->selected_sprite)
	{
		p1 = vec2i(editor->selected_sprite->pos.x, editor->selected_sprite->pos.y);
		p2 = vec2i(p1.x + editor->selected_sprite->pos.w, p1.y + editor->selected_sprite->pos.h);
		ui_surface_rect_draw(surface, p1, p2, 0xff0000ff);
	}

	// Draw lines on the surface borders for debugging purposes;
	ui_surface_draw_border(surface, 1, 0xff00ff00);

	SDL_UpdateTexture(editor->wall_render->texture,
		&(SDL_Rect){0, 0, ft_min(surface->w, editor->wall_render->current_texture_size.x),
			ft_min(surface->h, editor->wall_render->current_texture_size.y)},
		surface->pixels, surface->pitch);

	SDL_FreeSurface(surface);
}

void	sprite_events(t_editor *editor)
{
	char	temp_str[20];

	if (!editor->selected_wall || !editor->selected_sector)
	{
		ft_printf("[%s] We need both selected wall and sector for this function, and currently we dont have both so no function running for you.\n", __FUNCTION__);
		return ;
	}

	ft_strnclr(temp_str, 20);

	editor->sprite_edit_menu->show = 1;

	if (editor->sprite_add_button->state == UI_STATE_CLICK)
	{
		if (!editor->selected_sprite) // unselects if you have one selected.
		{
			editor->selected_sprite = add_sprite(editor);
			if (editor->active_texture_button_id > -1)
				editor->selected_sprite->texture = editor->active_texture_button_id;
			editor->selected_sprite->parent = editor->selected_wall;
			add_to_list(&editor->selected_wall->sprites, editor->selected_sprite, sizeof(t_sprite));
			++editor->selected_wall->sprite_amount;
		}
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
			ft_printf("Sprite Removed (total : %d)\n", editor->selected_wall->sprite_amount);
		}
	}

	// Wall Render
	render_wall_on_sprite_menu(editor, editor->selected_sector, editor->selected_wall);

	// Selecting Sprite
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& ui_element_is_hover(editor->wall_render))
	{
		t_vec2i		actual_mouse_pos;
		t_sprite	*sprite;

		actual_mouse_pos = vec2i(editor->win_main->mouse_pos.x - editor->wall_render->screen_pos.x,
				editor->win_main->mouse_pos.y - editor->wall_render->screen_pos.y);
		sprite = get_sprite_from_list_at_pos(editor->selected_wall->sprites, actual_mouse_pos);
		if (sprite) // new sprite found, update ui;
		{
			editor->selected_sprite = sprite;
			set_sprite_ui(editor, editor->selected_sprite);
		}
	}

	// Editing Sprite
	if (editor->selected_sprite)
	{
		t_vec2i	mouse_diff;

		mouse_diff = vec2i(editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x,
				editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y);
		// Moving
		if (editor->win_main->mouse_down == SDL_BUTTON_RIGHT // moving With mouse
			&& ui_element_is_hover(editor->sprite_edit_menu))
		{
			editor->selected_sprite->pos.x += mouse_diff.x;
			editor->selected_sprite->pos.y += mouse_diff.y;
			ui_input_set_text(editor->sprite_x_input, ft_b_itoa(editor->selected_sprite->pos.x, temp_str));
			ui_input_set_text(editor->sprite_y_input, ft_b_itoa(editor->selected_sprite->pos.y, temp_str));
		}
		get_sprite_ui(editor, editor->selected_sprite);
	}
}

void	select_wall(t_editor *editor)
{
	t_wall	*wall;

	// we dont want to make selected_wall NULL if we dont select a wall;
	wall = get_wall_from_list_around_radius(
			editor->selected_sector->walls, editor->mouse_pos, 1.0f);
	if (wall) // this is where we go when new wall selected;
	{
		editor->selected_wall = wall;
		editor->active_texture_button = NULL; // unselect currently selected texture;
		editor->selected_sprite = NULL;
		set_wall_ui(editor, wall); // fill the ui from this wall;
	}
}

void	wall_select_events(t_editor *editor)
{
	// Selecting wall;
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor)
		&& editor->selected_sector)
		select_wall(editor);
	else if (editor->selected_wall // MOVE WALL
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !(editor->wall_render->show == 1
		&& ui_element_is_hover(editor->sprite_edit_menu))) // only move wall if we are hovering over the sprite_edit_menu;
		move_wall(editor->selected_wall, editor->move_amount);
	// Close Wall Menu Button
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
		get_wall_ui(editor, editor->selected_wall); // fill this wall from ui;
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
			sprite_events(editor);
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

void	texture_menu_events(t_editor *editor)
{
	t_texture_elem	*selected_texture_elem;

	// Texture opening button events;
	if (ui_list_radio_event(editor->texture_opening_buttons, &editor->active_texture_opening_button))
	{
		// change text of the menu to the correct button;
		ui_label_set_text(editor->texture_menu_label, ui_button_get_text(editor->active_texture_opening_button));
		editor->active_texture_button = NULL;
		editor->active_texture_button_id = -1;
		editor->texture_menu->show = 1;
	}

	if (!editor->texture_menu->show)
		return ;

	if (editor->texture_menu_close_button->state == UI_STATE_CLICK)
	{
		editor->texture_menu->show = 0;
		editor->active_texture_opening_button->state = UI_STATE_DEFAULT;
		editor->active_texture_opening_button = NULL;
	}

	selected_texture_elem = NULL;
	// If new active;
	if (ui_list_radio_event(editor->texture_buttons, &editor->active_texture_button))
	{
		// Loop through texture_elem and compare which button we have clicked;	
		t_list	*curr;
		curr = editor->texture_elems;
		while (curr)
		{
			if (((t_texture_elem *)curr->content)->button == editor->active_texture_button)
			{
				selected_texture_elem = curr->content;
				editor->active_texture_button_id = selected_texture_elem->id;
				break ;
			}
			curr = curr->next;
		}
		// If texture found;
		if (selected_texture_elem
			&& editor->active_texture_button_id > -1)
		{
			// Set selected whatever // TODO this needs to go;
			if (editor->active_texture_opening_button == editor->wall_texture_button)
			{
				if (editor->selected_wall)
				{
					editor->selected_wall->wall_texture = selected_texture_elem->id;
					ui_element_image_set(editor->wall_texture_image, UI_STATE_AMOUNT, editor->wall_textures[selected_texture_elem->id]);
				}
			}
			else if (editor->active_texture_opening_button == editor->portal_texture_button)
			{
				if (editor->selected_wall)
				{
					editor->selected_wall->portal_texture = selected_texture_elem->id;
					ui_element_image_set(editor->portal_texture_image, UI_STATE_AMOUNT, editor->wall_textures[selected_texture_elem->id]);
				}
			}
			else if (editor->active_texture_opening_button == editor->floor_texture_button)
			{
				if (editor->selected_sector)
				{
					editor->selected_sector->floor_texture = selected_texture_elem->id;
					ui_element_image_set(editor->floor_texture_image, UI_STATE_AMOUNT, editor->wall_textures[selected_texture_elem->id]);
				}
			}
			else if (editor->active_texture_opening_button == editor->ceiling_texture_button)
			{
				if (editor->selected_sector)
				{
					editor->selected_sector->ceiling_texture = selected_texture_elem->id;
					ui_element_image_set(editor->ceiling_texture_image, UI_STATE_AMOUNT, editor->wall_textures[selected_texture_elem->id]);
				}
			}
			else if (editor->active_texture_opening_button == editor->sprite_texture_button)
			{
				if (editor->selected_sprite)
					editor->selected_sprite->texture = selected_texture_elem->id;
				ui_element_image_set(editor->sprite_texture_image, UI_STATE_AMOUNT, editor->wall_textures[selected_texture_elem->id]);
			}
		}
	}
}

void	select_entity(t_editor *editor)
{
	t_entity	*entity;

	entity = get_entity_from_list_around_radius(
			editor->entities, editor->mouse_pos, 1.0f);
	// we dont want to overwrite the currently selected entity if we dont actually find a new one;
	if (entity) // if new entity selected;
	{
		editor->selected_entity = entity;
		set_entity_ui(editor, editor->selected_entity);
	}
}

void	entity_select_events(t_editor *editor)
{
	// If the dropdown menu is open, lets ignore all other inputs;
	editor->entity_yaw_input->event = 1;
	editor->entity_yaw_slider->event = 1;
	editor->entity_z_input->event = 1;
	if (ui_dropdown_is_open(editor->entity_dropdown))
	{
		editor->entity_yaw_input->event = 0;
		editor->entity_yaw_slider->event = 0;
		editor->entity_z_input->event = 0;
	}
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
			&& !hover_over_open_menus(editor))
			add_entity(editor)->pos = editor->mouse_pos;
	}
	else if (editor->select_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
			&& !ui_element_is_hover(editor->entity_edit_menu))
			select_entity(editor);
		else if (editor->selected_entity // MOVE ENTITY;
			&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
			editor->selected_entity->pos = vec2i_add(
					editor->selected_entity->pos, editor->move_amount);
		// Fill entity with values from ui;
		if (editor->selected_entity)
			get_entity_ui(editor, editor->selected_entity);
		if (!editor->selected_entity
			|| editor->close_entity_edit_button->state == UI_STATE_CLICK)
		{
			editor->entity_edit_menu->show = 0;
			editor->selected_entity = NULL;
		}
		else
			editor->entity_edit_menu->show = 1;
	}
}

void	entity_events(t_editor *editor)
{
	if (editor->entity_button->state == UI_STATE_CLICK)
	{
		entity_select_events(editor);
		if (!editor->selected_entity)
			editor->entity_edit_menu->show = 0;
	}
	else
	{
		editor->selected_entity = NULL;
		editor->entity_edit_menu->show = 0;
	}
}

void	event_select_events(t_editor *editor)
{
	char	target_id_text[20];

	ft_strnclr(target_id_text, 20);

	// Ignore all ui inputs if we have dropdowns open;
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
	
	// Selecting event;
	// Lets see if we have gotten a new active event_elem;
	if (ui_list_radio_event(editor->event_element_buttons, &editor->active_event_elem))
	{
		// Figure out which event this button is attached to;
		t_list	*curr;
		curr = editor->event_elements;
		while (curr)
		{
			if (((t_event_elem *)curr->content)->button == editor->active_event_elem)
			{
				editor->selected_event_elem = curr->content;
				editor->selected_event = editor->selected_event_elem->event;
				break;
			}
			curr = curr->next;
		}
		// Update event ui with values from that event;
		if (editor->selected_event)
			set_event_ui(editor, editor->selected_event);
	}

	// If add button is clicked;
	if (editor->add_event_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_event_elem == NULL) // add new element;
		{
			editor->selected_event = add_event(editor);
			editor->selected_event_elem = editor->selected_event->elem;
			editor->active_event_elem = editor->selected_event->elem->button;
		} // else update, currently selected, element from the ui;
		update_event(editor, editor->selected_event_elem->event);
		update_event_elem(editor->selected_event_elem);

		editor->selected_event_elem = NULL;
		editor->active_event_elem = NULL;
		editor->selected_event = NULL;
	} // Else if remove button is clicked;
	else if (editor->remove_event_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_event && editor->selected_event_elem)
		{
			remove_event(editor, editor->selected_event);
			editor->selected_event_elem = NULL;
			editor->active_event_elem = NULL;
			editor->selected_event = NULL;
		}
	}

	// Update ID Dropdown;
	// when id dropdown opens we add buttons for each sprite/sector id,
	// 	depending on which action type is selected;
	if (editor->event_id_dropdown->show
		&& ui_dropdown_open(editor->event_id_dropdown))
	{
		char	**texts;

		if (ui_dropdown_active(editor->event_action_dropdown) == editor->event_action_sector)
		{
			texts = gen_sector_id_texts(editor->sectors);
			t_ui_recipe *recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "event_id_button");
			create_buttons_to_list_from_texts_remove_extra(ui_dropdown_get_menu_element(editor->event_id_dropdown), texts, recipe);
			ft_arraydel(texts);
		}
		else
		{
			texts = gen_sprite_id_texts(editor->sprites);
			t_ui_recipe *recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "event_id_button");
			create_buttons_to_list_from_texts_remove_extra(ui_dropdown_get_menu_element(editor->event_id_dropdown), texts, recipe);
			ft_arraydel(texts);
		}
	}
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
	{
		editor->spawn_edit_menu->show = 0;
		return ;
	}
	ft_strnclr(temp_str, 20);
	// Update ui same frame the button is pressed;
	if (editor->spawn_button->was_click)
		ui_input_set_text(editor->spawn_yaw_input, ft_b_itoa(editor->spawn.yaw, temp_str));
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (!hover_over_open_menus(editor))
			if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
				editor->spawn.pos = editor->mouse_pos;
	}
	else if (editor->select_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
			editor->spawn.pos = vec2i_add(editor->spawn.pos, editor->move_amount);
	}
	// Yaw input
	if (ui_input_exit(editor->spawn_yaw_input))
	{
		editor->spawn.yaw = ft_clamp(ft_atoi(ui_input_get_text(editor->spawn_yaw_input)), 0, 360);
		ui_input_set_text(editor->spawn_yaw_input, ft_b_itoa(editor->spawn.yaw, temp_str));
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
	// Only one of point or walls should be changing the info text;
	if (!update_point_info(editor, editor->selected_point))
		if (update_wall_info(editor, editor->selected_wall))
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
		if (editor->map_type == 1)
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
	if (editor->endless_checkbox->was_click)
		editor->map_type = 0;
	else if (editor->story_checkbox->was_click)
		editor->map_type = 1;
	ui_checkbox_toggle_accordingly(editor->endless_checkbox,
		editor->map_type == 0);
	ui_checkbox_toggle_accordingly(editor->story_checkbox,
		editor->map_type == 1);
	if (!editor->win_save->show)
		return ;
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