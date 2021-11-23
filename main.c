#include "editor.h"

/*
 * This calculates the actual position in game for the mouse;
*/
void	calculate_hover(t_editor *editor)
{
	editor->last_mouse_pos = editor->mouse_pos;

	editor->mouse_pos.x = (editor->win_main->mouse_pos.x / (editor->gap_size * editor->zoom)) + editor->offset.x;
	editor->mouse_pos.y = (editor->win_main->mouse_pos.y / (editor->gap_size * editor->zoom)) + editor->offset.y;

	editor->move_amount.x = editor->mouse_pos.x - editor->last_mouse_pos.x;
	editor->move_amount.y = editor->mouse_pos.y - editor->last_mouse_pos.y;
}

/*
 * From in game position calculate the screen position;
*/
void	draw_hover(t_editor *editor)
{
	t_vec2i	convert_back;

	convert_back.x = (editor->mouse_pos.x - editor->offset.x) * (editor->gap_size * editor->zoom);
	convert_back.y = (editor->mouse_pos.y - editor->offset.y) * (editor->gap_size * editor->zoom);
	ui_surface_circle_draw_filled(editor->drawing_surface, convert_back, 3, 0xffffffff);

	// Crosshair
	ui_surface_line_draw(editor->drawing_surface,
		vec2i(convert_back.x, 0),
		vec2i(convert_back.x, editor->drawing_surface->h),
		0x70b0b0b0);
	ui_surface_line_draw(editor->drawing_surface,
		vec2i(0, convert_back.y),
		vec2i(editor->drawing_surface->w, convert_back.y),
		0x70b0b0b0);

	// Draw from first point to where you're hovering;
	if (editor->first_point_set)
		ui_surface_line_draw(editor->drawing_surface,
			conversion(editor, editor->first_point),
			convert_back, 0xffffff00);
}

t_wall	*get_sector_wall_at_pos(t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_list	*curr;
	t_wall	*curr_wall;

	curr = sector->walls;
	while (curr)
	{
		curr_wall = curr->content;
		if ((compare_veci(curr_wall->p1->pos.v, p1.v, 2)
			&& compare_veci(curr_wall->p2->pos.v, p2.v, 2))
			|| (compare_veci(curr_wall->p1->pos.v, p2.v, 2)
			&& compare_veci(curr_wall->p2->pos.v, p1.v, 2)))
			return (curr_wall);
		curr = curr->next;
	}
	return (NULL);
}

bool	can_you_make_portal_of_this_wall(t_list *sector_list, t_sector *part_of_sector, t_wall *wall)
{
	t_sector	*sector;
	t_wall		*temp_wall;

	if (!part_of_sector || !wall)
		return (0);
	while (sector_list)
	{
		sector = sector_list->content;
		if (sector != part_of_sector)
		{
			temp_wall = get_sector_wall_at_pos(sector, wall->p1->pos, wall->p2->pos);
			if (temp_wall)
			{
				wall->neighbor = sector;
				temp_wall->neighbor = part_of_sector;
				return (1);
			}
		}
		sector_list = sector_list->next;
	}
	return (0);
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
 * 2.5. Remove all neighbors not connected; (if a wall has neighbor but it's corresponding neighbor/sector doesn't have same wall as neighbor, or at all);
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

/*
 * NOTE: Basically sector_drawing_events, not sure yet if we want to rename it that;
*/
void	sector_events(t_editor *editor, SDL_Event e)
{
	if (editor->draw_button->state == UI_STATE_CLICK
		&& !hover_over_open_menus(editor)
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
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
				add_wall_to_sector_at_pos(editor, editor->selected_sector, editor->first_point, editor->second_point);
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

void	point_events(t_editor *editor)
{
	t_point	*point;

	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
	{
		point = get_point_from_sector_around_radius(editor->selected_sector, editor->mouse_pos, 1.0f);
		if (point)
			editor->selected_point = point;
	}
	else if (editor->selected_point && editor->win_main->mouse_down == SDL_BUTTON_RIGHT) // MOVE POINT
		editor->selected_point->pos = vec2i_add(editor->selected_point->pos, editor->move_amount);
}

void	sector_edit_events(t_editor *editor)
{
	t_sector	*sector;

	// Select sector;
	if (editor->select_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
	{
		// We dont want to overwrite the currently selected sector with NULL if we dont find sector on mouseclick pos;
		sector = get_sector_from_list_around_radius(editor->sectors, editor->mouse_pos, 1);
		if (sector) // if new sector selected;
		{
			editor->selected_sector = sector;
			editor->selected_wall = NULL;
			editor->selected_point = NULL;
			editor->active_texture_button = NULL; // unselect currently selected texture;

			set_sector_ui(editor, editor->selected_sector);
		}
	}
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

void	wall_events(t_editor *editor, SDL_Event e)
{
	// Selecting wall;
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor)
		&& editor->selected_sector)
	{
		t_wall	*wall;
		// we dont want to make selected_wall NULL if we dont select a wall;
		wall = get_wall_from_list_around_radius(editor->selected_sector->walls, editor->mouse_pos, 1.0f);
		if (wall) // this is where we go when new wall selected;
		{
			editor->selected_wall = wall;
			editor->active_texture_button = NULL; // unselect currently selected texture;
			editor->selected_sprite = NULL;

			set_wall_ui(editor, wall); // fill the ui from this wall;
		}
	}
	else if (editor->selected_wall // MOVE WALL
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !(editor->wall_render->show == 1
		&& ui_element_is_hover(editor->sprite_edit_menu))) // only move wall if we are hovering over the sprite_edit_menu;
	{
		editor->selected_wall->p1->pos = vec2i_add(editor->selected_wall->p1->pos, editor->move_amount);
		editor->selected_wall->p2->pos = vec2i_add(editor->selected_wall->p2->pos, editor->move_amount);
	}

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

void	entity_events(t_editor *editor, SDL_Event e)
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
		{
			t_entity	*entity = add_entity(editor);
			entity->pos = editor->mouse_pos;
			//add_entity(editor)->pos = editor->mouse_pos; // change to this if you have to;
		}
	}
	else if (editor->select_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
			&& !ui_element_is_hover(editor->entity_edit_menu))
		{
			t_entity *entity = get_entity_from_list_around_radius(editor->entities, editor->mouse_pos, 1.0f);
			// we dont want to overwrite the currently selected entity if we dont actually find a new one;
			if (entity) // if new entity selected;
			{
				editor->selected_entity = entity;

				set_entity_ui(editor, editor->selected_entity);
			}
		}
		else if (editor->selected_entity // MOVE ENTITY;
			&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
			editor->selected_entity->pos = vec2i_add(editor->selected_entity->pos, editor->move_amount);

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

void	event_events(t_editor *editor, SDL_Event e)
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

// 		create buttons to 'parent->children' with the texts from 'texts', all buttons have 'recipe' recipe;
void	create_buttons_to_list_from_texts_remove_extra(t_ui_element *parent, char **texts, t_ui_recipe *recipe)
{
	int			i;
	t_list		*prev_butt;

	i = -1;
	prev_butt = parent->children;
	while (texts[++i])
	{
		if (prev_butt && prev_butt->content) // we have button;
		{
			ui_button_set_text(prev_butt->content, texts[i]);
			prev_butt = prev_butt->next;
		}
		else // create new button to list end and add ui_element as content;
		{
			t_ui_element *elem = ft_memalloc(sizeof(t_ui_element));
			ui_button_new(parent->win, elem);
			ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
			ui_element_edit(elem, recipe);
			ui_button_set_text(elem, texts[i]);
		}
	}
	// Finally remove all the extra buttons;
	while (prev_butt)
	{
		ui_element_free(prev_butt->content);
		free(prev_butt->content);
		prev_butt = prev_butt->next;
	}
}

char	**gen_sector_id_texts(t_list *sectors)
{
	char		**arr;
	char		temp[20];
	t_sector	*sector;

	arr = NULL;
	while (sectors)
	{
		sector = sectors->content;
		arr = ft_arradd(arr, ft_b_itoa(sector->id, temp));
		sectors = sectors->next;
	}
	return (arr);
}

char	**gen_sprite_id_texts(t_list *sprites)
{
	char		**arr;
	char		temp[20];
	t_sprite	*sprite;

	arr = NULL;
	while (sprites)
	{
		sprite = sprites->content;
		arr = ft_arradd(arr, ft_b_itoa(sprite->id, temp));
		sprites = sprites->next;
	}
	return (arr);
}

void	spawn_events(t_editor *editor, SDL_Event e)
{
	char	temp_str[20];

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
		final_str = ft_sprintf("%d, %d", editor->mouse_pos.x, editor->mouse_pos.y);
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

void	sector_hover_info_events(t_editor *editor, SDL_Event e)
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
	temp = ft_sprintf("Sector Info\n\nID: %d\n%27cFloor-%7cCeiling-\nHeight : %14d %16d\nTexture : %12d %16d\nTexture Scale : %2.2f %12.2f\n",
			sector->id, ' ', ' ', sector->floor_height, sector->ceiling_height, sector->floor_texture, sector->ceiling_texture, sector->floor_scale, sector->ceiling_scale);
	ui_label_set_text(editor->sector_hover_info_label, temp);
	ft_strdel(&temp);
	ui_element_pos_set2(editor->sector_hover_info_menu,
		vec2(editor->win_main->mouse_pos.x + 10, editor->win_main->mouse_pos.y + 10));
}

void	send_info_message(t_editor *editor, char *text)
{
	editor->info_label_start_timer = SDL_GetTicks();
	editor->info_label_timer = 0;
	ui_label_color_set(editor->info_label, 0xffffffff);
	ui_label_set_text(editor->info_label, text);
	editor->info_label->show = 1;
}

void	update_info_label(t_editor *editor)
{
	t_rgba	new_col;

	if (editor->info_label->show
		&& SDL_GetTicks() - editor->info_label_start_timer >= editor->info_label_timer)
	{
		new_col = hex_to_rgba(ui_label_get_color(editor->info_label));
		new_col.a -= 2;
		ui_label_color_set(editor->info_label, rgba_to_hex(new_col));
		if (new_col.a <= 0)
			editor->info_label->show = 0;
	}
}

void	save_window_events(t_editor *editor, SDL_Event e)
{
	if (editor->save_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_save, UI_WINDOW_SHOW | UI_WINDOW_RAISE);
	if (editor->endless_checkbox->was_click)
		editor->map_type = 0;
	else if (editor->story_checkbox->was_click)
		editor->map_type = 1;
	ui_checkbox_toggle_accordingly(editor->endless_checkbox, editor->map_type == 0);
	ui_checkbox_toggle_accordingly(editor->story_checkbox, editor->map_type == 1);
	if (!editor->win_save->show)
		return ;
	if (ui_button(editor->confirm_save_button))
	{
		if (ft_strlen(ui_input_get_text(editor->name_input)) < 1)
			ft_printf("[%s] Map name len is less than 1, won\'t slide.\n", __FUNCTION__);
		else
		{
			char	*actual_full_path;

			if (editor->map_type == 1)
				actual_full_path = ft_strjoiner(MAP_PATH, ui_input_get_text(editor->name_input), ".dnds", NULL);
			else
				actual_full_path = ft_strjoiner(MAP_PATH, ui_input_get_text(editor->name_input), ".dnde", NULL);
			set_map(editor, actual_full_path);
			ft_strdel(&actual_full_path);
			ui_window_flag_set(editor->win_save, UI_WINDOW_HIDE);
			send_info_message(editor, "Map Saved Successfully!");
		}
	}
}

void	edit_window_events(t_editor *editor, SDL_Event e)
{
	if (editor->edit_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_edit, UI_WINDOW_SHOW | UI_WINDOW_RAISE);
	if (!editor->win_edit->show)
		return ;
	if (ui_input_exit(editor->map_scale_input))
		editor->map_scale = ft_atof(ui_input_get_text(editor->map_scale_input));
}

void	render_wall_on_sprite_menu(t_editor *editor, t_sector *sector, t_wall *wall)
{
	// Clear the wall render from last frame, since we dont draw over the whole texture;
	ui_texture_fill(editor->win_main->renderer, editor->wall_render->texture, 0xff000000);

	SDL_Surface	*surface;
	t_vec2		dist;
	float		aspect;

	dist.x = distance(wall->p1->pos.v, wall->p2->pos.v, 2);
	dist.y = (sector->ceiling_height - sector->floor_height);
	int		size = 64;
	float	amount_x = dist.x / wall->texture_scale;
	float	amount_y = dist.y / wall->texture_scale;
	aspect = get_ratio_f(vec2(dist.x * size, dist.y * size), vec2(editor->wall_render->pos.w, editor->wall_render->pos.h));

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

void	sprite_events(t_editor *editor, SDL_Event e)
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
			editor->selected_sprite = sprite_new();
			editor->selected_sprite->parent = editor->selected_wall;
			editor->selected_sprite->pos = vec4i(0, 0, 64, 64);
			editor->selected_sprite->texture = 0;
			if (editor->active_texture_button_id > -1)
				editor->selected_sprite->texture = editor->active_texture_button_id;
			editor->selected_sprite->id = get_next_sprite_id(editor->sprites);
			editor->selected_sprite->scale = 1.0f;
			add_to_list(&editor->selected_wall->sprites, editor->selected_sprite, sizeof(t_sprite));
			add_to_list(&editor->sprites, editor->selected_sprite, sizeof(t_sprite));
			++editor->selected_wall->sprite_amount;
			++editor->sprite_amount;
			ft_printf("New Sprite Added (%d)\n", editor->selected_wall->sprite_amount);
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
			--editor->sprite_amount;
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

			// TODO: update ui;

			ui_input_set_text(editor->sprite_scale_input, ft_b_ftoa(sprite->scale, 2, temp_str));
			if (editor->selected_sprite->type == STATIC)
				ui_dropdown_activate(editor->sprite_type_dropdown, editor->sprite_type_static);
			else if (editor->selected_sprite->type == LOOP)
				ui_dropdown_activate(editor->sprite_type_dropdown, editor->sprite_type_loop);
			else if (editor->selected_sprite->type == ACTION)
				ui_dropdown_activate(editor->sprite_type_dropdown, editor->sprite_type_action);
			ui_input_set_text(editor->sprite_x_input, ft_b_itoa(sprite->pos.x, temp_str));
			ui_input_set_text(editor->sprite_y_input, ft_b_itoa(sprite->pos.y, temp_str));
		}
	}

	// Editing Sprite
	if (editor->selected_sprite)
	{
		// Moving
		if (editor->win_main->mouse_down == SDL_BUTTON_RIGHT // moving With mouse
			&& ui_element_is_hover(editor->sprite_edit_menu))
		{
			editor->selected_sprite->pos.x = editor->selected_sprite->pos.x + editor->move_amount.x;
			editor->selected_sprite->pos.y = editor->selected_sprite->pos.y + editor->move_amount.y;
			ui_input_set_text(editor->sprite_x_input, ft_b_itoa(editor->selected_sprite->pos.x, temp_str));
			ui_input_set_text(editor->sprite_y_input, ft_b_itoa(editor->selected_sprite->pos.y, temp_str));
		}
		if (ui_input_exit(editor->sprite_x_input)) // moving with inputs
			editor->selected_sprite->pos.x = ft_atoi(ui_input_get_text(editor->sprite_x_input));
		if (ui_input_exit(editor->sprite_y_input))
			editor->selected_sprite->pos.y = ft_atoi(ui_input_get_text(editor->sprite_y_input));

		// Scale
		if (ui_input_exit(editor->sprite_scale_input))
			editor->selected_sprite->scale = ft_atof(ui_input_get_text(editor->sprite_scale_input));

		if (ui_dropdown_exit(editor->sprite_type_dropdown))
		{
			if (editor->sprite_type_loop->state == UI_STATE_CLICK)
				editor->selected_sprite->type = LOOP;
			else if (editor->sprite_type_action->state == UI_STATE_CLICK)
				editor->selected_sprite->type = ACTION;
			else
				editor->selected_sprite->type = STATIC;
		}
	}
}

void	texture_menu_events(t_editor *editor, SDL_Event e)
{
	t_texture_elem	*selected_texture_elem;

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
			// Set selected whatever
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

void	user_events(t_editor *editor, SDL_Event e)
{
	char	temp_str[20];

	ft_strnclr(temp_str, 20);

	calculate_hover(editor);

	// Info Label events
	update_info_label(editor);

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
	if (ui_button(editor->draw_button))
	{
		editor->selected_sector = NULL;
		editor->selected_entity = NULL;
		editor->selected_event = NULL;
		editor->selected_sprite = NULL;
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}

	// Remove button (dont remove it, it's its name)
	if (editor->remove_button->state == UI_STATE_CLICK)
	{
		int	was_removed = 0;
		/* Use this once they go through the parent and remove itself from their list too! (crash otherwise);
		if (editor->selected_point)
			was_removed = remove_point(editor, editor->selected_point);
		else if (editor->selected_wall)
			was_removed = remove_wall(editor, editor->selected_wall);
		else*/ if (editor->selected_sector)
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

	// Sector Events
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
			sector_events(editor, e);
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
	}

	// Wall Events
	//if (editor->selected_sector
	if (editor->sector_button->state == UI_STATE_CLICK
		&& editor->wall_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_wall)
		{
			editor->menu_wall_edit->show = 1;
			sprite_events(editor, e);
		}
		else
		{
			editor->selected_sprite = NULL;
			editor->menu_wall_edit->show = 0;
			editor->sprite_edit_menu->show = 0;
		}
		wall_events(editor, e);
	}
	else
	{
		editor->selected_wall = NULL;
		editor->menu_wall_edit->show = 0;
		editor->selected_sprite = NULL;
		editor->sprite_edit_menu->show = 0;
	}

	// Point Events
	if (editor->sector_button->state == UI_STATE_CLICK
		&& editor->point_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_sector)
		{
			if (editor->point_button->state == UI_STATE_CLICK)
				point_events(editor);
			else
				editor->selected_point = NULL;
		}
		else
			editor->selected_point = NULL;
	}



	if (ui_list_radio_event(editor->texture_opening_buttons, &editor->active_texture_opening_button))
	{
		// change text of the menu to the correct button;
		ui_label_set_text(editor->texture_menu_label, ui_button_get_text(editor->active_texture_opening_button));
		editor->active_texture_button = NULL;
		editor->active_texture_button_id = -1;
		editor->texture_menu->show = 1;
	}

	if (editor->texture_menu->show)
		texture_menu_events(editor, e);

	if (editor->entity_button->state == UI_STATE_CLICK)
		entity_events(editor, e);
	else
	{
		editor->selected_entity = NULL;
		editor->entity_edit_menu->show = 0;
	}

	if (editor->event_button->state == UI_STATE_CLICK)
	{
		editor->event_edit_menu->show = 1;
		event_events(editor, e);
	}
	else
	{
		editor->selected_event = NULL;
		editor->event_edit_menu->show = 0;
	}

	if (editor->spawn_button->state == UI_STATE_CLICK)
	{
		editor->spawn_edit_menu->show = 1;
		spawn_events(editor, e);
	}
	else
		editor->spawn_edit_menu->show = 0;

	info_menu_events(editor, e);
	sector_hover_info_events(editor, e);
	save_window_events(editor, e);
	edit_window_events(editor, e);
}

void	draw_grid(SDL_Surface *surface, float gap_size, float zoom)
{
	int			w_amount;
	int			h_amount;
	int			i;

	w_amount = surface->w / (gap_size * zoom);
	h_amount = surface->h / (gap_size * zoom);
	i = 0;
//	SDL_FillRect(surface, NULL, 0xff000000); // fill the surface first;
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
		if (wall->neighbor)
		{
			if (get_sector_wall_at_pos(wall->neighbor, wall->p1->pos, wall->p2->pos))
				wall_render(editor, wall, 0xffff0000);
			else
				wall->neighbor = NULL;
		}
		else
			wall_render(editor, wall, color);
		point_render(editor, wall->p1, color);
		walls = walls->next;
	}
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
		text_surface = TTF_RenderText_Blended(font, text, (SDL_Color){rgba.r, rgba.g, rgba.b, rgba.a});
		SDL_BlitSurface(text_surface, NULL, surface, 
			&(SDL_Rect){pos.x - (text_surface->w / 2), pos.y - (text_surface->h / 2),
			text_surface->w, text_surface->h});
		SDL_FreeSurface(text_surface);
	}
	else
		ft_printf("[%s] Failed drawing text \"%s\" no font.\n", __FUNCTION__, text);
}

/*
 * NOTE: this doesnt work on texture that update every frame;
 * NOTE: im not 100% sure this works;
 * !! IMPORTANT !!! ^
*/
void	draw_text_on_texture(SDL_Texture *texture, char *text, TTF_Font *font, t_vec2i pos, Uint32 color)
{
	SDL_Surface	*text_surface;
	t_rgba		rgba;
	int			t_w;
	int			t_h;
	SDL_Rect	rect;

	if (font)
	{
		rgba = hex_to_rgba(color);
		TTF_SetFontHinting(font, TTF_HINTING_MONO);
		text_surface = TTF_RenderText_Blended(font, text, (SDL_Color){rgba.r, rgba.g, rgba.b, rgba.a});
		// TODO : if this is a function that will stay for a longer time, consider chekcing the format to be the same as the one that the ttf outputs on the surface;
		SDL_QueryTexture(texture, NULL, NULL, &t_w, &t_h);
		rect.x = ft_clamp(pos.x - (text_surface->w / 2), 0, t_w);
		rect.y = ft_clamp(pos.y - (text_surface->h / 2), 0, t_h);
		rect.w = ft_clamp(text_surface->w, 0, t_w - rect.x);
		rect.h = ft_clamp(text_surface->h, 0, t_h - rect.y);
		SDL_UpdateTexture(texture, &rect, text_surface->pixels, text_surface->pitch);
		SDL_FreeSurface(text_surface);
	}
	else
		ft_printf("[%s] Failed drawing text \"%s\" no font.\n", __FUNCTION__, text);
}



void	draw_sectors(t_editor *editor, t_list *sectors)
{
	t_sector	*sector;
	char		temp[20];
	t_vec2i		converted_center;
	t_sector	*hovered_this_frame;

	hovered_this_frame = NULL;
	while (sectors)
	{
		sector = sectors->content;
		sort_walls(sector->walls); // TODO do this everytime the sector has been updated, not every frame (if its slow);
		sector_render(editor, sector, sector->color);
		sector->center = get_sector_center(sector);
		converted_center = conversion(editor, sector->center);
		ft_b_itoa(sector->id, temp);
		draw_text(editor->drawing_surface, temp, editor->font, converted_center, 0xffffffff);
		if (!check_sector_convexity(sector))
		{
			draw_text(editor->drawing_surface, "Not Convex!", editor->font, converted_center, 0xffff0000); 
			editor->errors += 1;
		}
		if (sector->ceiling_height - sector->floor_height < 0)
		{
			draw_text(editor->drawing_surface, "Floor & Ceiling Height Doesn\'t Make Sense!", editor->font, converted_center, 0xffffff00); 
			editor->errors += 1;
		}
		// Get hovered
		if (vec2_in_vec4(editor->mouse_pos,
			vec4i(sector->center.x - 1, sector->center.y - 1, 3, 3)))
			hovered_this_frame = sector;
		sectors = sectors->next;
	}
	editor->hovered_sector = hovered_this_frame;
}

void	draw_entities(t_editor *editor, t_list *entities)
{
	while (entities)
	{
		entity_render(editor, entities->content);
		entities = entities->next;
	}
}

void	draw_entities_yaw(t_editor *editor, t_list *entities)
{
	t_entity	*entity;
	char		temp_str[20];

	ft_strnclr(temp_str, 20);
	while (entities)
	{
		entity = entities->content;
		entity_yaw_render(editor, entities->content);

		// Check if entity inside a sector;
		t_list *curr = editor->sectors;
		entity->inside_sector = NULL;
		while (curr)
		{
			if (check_point_in_sector(curr->content, entity->pos) == 1)
			{
				entity->inside_sector = curr->content;
				break ;
			}
			curr = curr->next;
		}
		// Error showing;
		if (!entity->inside_sector)
		{
			draw_text(editor->drawing_surface, "Not Inside Sector!",
				editor->font, conversion(editor, entity->pos), 0xffff0000); 
			editor->errors += 1;
		}
		else if (entity->inside_sector
			&& (entity->z < entity->inside_sector->floor_height || entity->z > entity->inside_sector->ceiling_height))
		{
			draw_text(editor->drawing_surface, "Z not between Floor & Ceiling!",
				editor->font, conversion(editor, entity->pos), 0xffff0000); 
			editor->errors += 1;
		}

		entities = entities->next;
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
	if (editor->selected_entity)
	{
		t_vec2i	conv = conversion(editor, editor->selected_entity->pos);
		t_vec2i	p1 = vec2i(conv.x - 10, conv.y - 10);
		t_vec2i	p2 = vec2i(conv.x + 10, conv.y + 10);
		ui_surface_rect_draw(editor->drawing_surface, p1, p2, 0xffffff00);
	}
}

void	draw_spawn(t_editor *editor)
{
	ui_surface_circle_draw_filled(editor->drawing_surface,
		conversion(editor, editor->spawn.pos), 10, 0xff00ff00);

	t_list		*curr = editor->sectors;
	editor->spawn.inside_sector = NULL;
	while (curr)
	{
		if (check_point_in_sector(curr->content, editor->spawn.pos) == 1)
		{
			editor->spawn.inside_sector = curr->content;
			break ;
		}
		curr = curr->next;
	}
	if (!editor->spawn.inside_sector)
	{
		draw_text(editor->drawing_surface, "Not Inside Sector!",
			editor->font, conversion(editor, editor->spawn.pos), 0xffff0000); 
		editor->errors += 1;
	}
	if (editor->spawn.inside_sector)
		editor->spawn.z = editor->spawn.inside_sector->floor_height;
}

void	user_render(t_editor *editor)
{
	editor->errors = 0;
	// Grid stuff
	if (editor->update_grid)
	{
		draw_grid(editor->grid_surface, editor->gap_size, editor->zoom);
		editor->update_grid = 0;
		ft_printf("[%s] We are updating grid surface.\n", __FUNCTION__);
	}
	SDL_BlitSurface(editor->grid_surface, NULL, editor->drawing_surface, NULL);

	draw_sectors(editor, editor->sectors);
	draw_hover(editor);
	draw_selected(editor);

	draw_entities_yaw(editor, editor->entities);
	draw_spawn(editor);

	SDL_UpdateTexture(editor->drawing_texture, NULL, editor->drawing_surface->pixels, editor->drawing_surface->pitch);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer, editor->drawing_texture, NULL, NULL);

	// NOTE: this is the only thing drawn on the texture;
	draw_entities(editor, editor->entities);

	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
	SDL_FillRect(editor->drawing_surface, NULL, 0);

	if (editor->errors > 0)
	{
		char	*temp;
		temp = ft_sprintf("Consider fixing the %d error(s) before saving!", editor->errors);
		ui_label_set_text(editor->error_label, temp);
		ft_strdel(&temp);
		editor->error_label->show = 1;
	}
	else
		editor->error_label->show = 0;
}

void	editor_init(t_editor *editor)
{
	memset(editor, 0, sizeof(t_editor));
	ui_layout_load(&editor->layout, EDITOR_PATH"layout.ui");
	editor->spawn.yaw = 72; // REMOVE

	// Main Window
	editor->win_main = ui_list_get_window_by_id(editor->layout.windows, "win_main");

	// Selection Menu
	editor->menu_toolbox_top = ui_list_get_element_by_id(editor->layout.elements, "menu_toolbox_top");
	editor->menu_selection = ui_list_get_element_by_id(editor->layout.elements, "menu_select_buttons");
	editor->selection_dropdown_menu = ui_list_get_element_by_id(editor->layout.elements, "type_drop_menu");
	editor->draw_button = ui_list_get_element_by_id(editor->layout.elements, "draw_button");
	editor->select_button = ui_list_get_element_by_id(editor->layout.elements, "select_button");
	editor->remove_button = ui_list_get_element_by_id(editor->layout.elements, "remove_button");
	editor->point_button = ui_list_get_element_by_id(editor->layout.elements, "point_button");
	editor->wall_button = ui_list_get_element_by_id(editor->layout.elements, "wall_button");
	editor->sector_button = ui_list_get_element_by_id(editor->layout.elements, "sector_button");
	editor->entity_button = ui_list_get_element_by_id(editor->layout.elements, "entity_button");
	editor->spawn_button = ui_list_get_element_by_id(editor->layout.elements, "spawn_button");
	editor->event_button = ui_list_get_element_by_id(editor->layout.elements, "event_button");
	editor->save_button = ui_list_get_element_by_id(editor->layout.elements, "save_button");
	editor->edit_button = ui_list_get_element_by_id(editor->layout.elements, "edit_button");

	editor->sector_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "sector_edit_menu");
	editor->close_sector_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_sector_edit_button");
	editor->sector_edit_ok_button = ui_list_get_element_by_id(editor->layout.elements, "sector_edit_ok_button");
	editor->floor_texture_button = ui_list_get_element_by_id(editor->layout.elements, "floor_texture_button");
	editor->ceiling_texture_button = ui_list_get_element_by_id(editor->layout.elements, "ceiling_texture_button");
	editor->floor_texture_image = ui_list_get_element_by_id(editor->layout.elements, "floor_texture_image");
	editor->ceiling_texture_image = ui_list_get_element_by_id(editor->layout.elements, "ceiling_texture_image");
	add_to_list(&editor->texture_opening_buttons, editor->floor_texture_button, UI_TYPE_ELEMENT);
	add_to_list(&editor->texture_opening_buttons, editor->ceiling_texture_button, UI_TYPE_ELEMENT);
	// Inputs;
	editor->floor_height_input = ui_list_get_element_by_id(editor->layout.elements, "floor_height_input");
	editor->ceiling_height_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_height_input");
	editor->gravity_input = ui_list_get_element_by_id(editor->layout.elements, "gravity_input");
	editor->lighting_input = ui_list_get_element_by_id(editor->layout.elements, "lighting_input");
	editor->floor_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "floor_texture_scale_input");
	editor->ceiling_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_texture_scale_input");

	// Error
	editor->error_label = ui_list_get_element_by_id(editor->layout.elements, "error_label");

	// Wall
	editor->menu_wall_edit = ui_list_get_element_by_id(editor->layout.elements, "menu_wall_edit");
	editor->close_wall_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_wall_edit_button");
	editor->solid_checkbox = ui_list_get_element_by_id(editor->layout.elements, "solidity_checkbox");
	editor->portal_checkbox = ui_list_get_element_by_id(editor->layout.elements, "portal_checkbox");
	editor->split_wall_button = ui_list_get_element_by_id(editor->layout.elements, "split_wall_button");
	editor->wall_texture_button = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_button");
	editor->wall_texture_image = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_image");
	editor->portal_texture_button = ui_list_get_element_by_id(editor->layout.elements, "portal_texture_button");
	editor->portal_texture_image = ui_list_get_element_by_id(editor->layout.elements, "portal_texture_image");
	add_to_list(&editor->texture_opening_buttons, editor->wall_texture_button, UI_TYPE_ELEMENT);
	add_to_list(&editor->texture_opening_buttons, editor->portal_texture_button, UI_TYPE_ELEMENT);
	editor->floor_wall_angle_input = ui_list_get_element_by_id(editor->layout.elements, "floor_wall_angle_input");
	editor->ceiling_wall_angle_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_wall_angle_input");
	editor->wall_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_scale_input");

	// Sprite
	editor->sprite_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "sprite_edit_menu");
	editor->sprite_add_button = ui_list_get_element_by_id(editor->layout.elements, "sprite_add_button");
	editor->sprite_remove_button = ui_list_get_element_by_id(editor->layout.elements, "sprite_remove_button");
	editor->sprite_scale_input = ui_list_get_element_by_id(editor->layout.elements, "sprite_scale_input");
	editor->sprite_type_dropdown = ui_list_get_element_by_id(editor->layout.elements, "sprite_type_dropdown");
	editor->sprite_type_static = ui_list_get_element_by_id(editor->layout.elements, "sprite_type_static");
	editor->sprite_type_loop = ui_list_get_element_by_id(editor->layout.elements, "sprite_type_loop");
	editor->sprite_type_action = ui_list_get_element_by_id(editor->layout.elements, "sprite_type_action");
	editor->sprite_texture_button = ui_list_get_element_by_id(editor->layout.elements, "sprite_texture_button");
	add_to_list(&editor->texture_opening_buttons, editor->sprite_texture_button, UI_TYPE_ELEMENT);
	editor->sprite_texture_image = ui_list_get_element_by_id(editor->layout.elements, "sprite_texture_image");
	editor->sprite_x_input = ui_list_get_element_by_id(editor->layout.elements, "sprite_x_input");
	editor->sprite_y_input = ui_list_get_element_by_id(editor->layout.elements, "sprite_y_input");
	editor->wall_render = ui_list_get_element_by_id(editor->layout.elements, "wall_render");
	for (int i = 0; i < MAP_TEXTURE_AMOUNT; i++)
	{
		ft_printf("Load Image (%d) : %s\n", i, g_map_textures[i].path);
		editor->wall_textures[i] = load_bxpm_to_surface(g_map_textures[i].path);
	}

	// Texture Menu
	editor->texture_menu = ui_list_get_element_by_id(editor->layout.elements, "texture_menu");
	editor->texture_menu->show = 0;
	editor->texture_menu_close_button = ui_list_get_element_by_id(editor->layout.elements, "texture_menu_close_button");
	editor->texture_menu_label = ui_list_get_element_by_id(editor->layout.elements, "texture_menu_label");
	t_ui_recipe	*texture_menu_button_recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "texture_button_menu");
	t_ui_recipe	*texture_button_recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "texture_button");
	t_ui_recipe	*texture_image_recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "texture_image");
	for (int i = 0; i < MAP_TEXTURE_AMOUNT; i++)
	{
		t_texture_elem	*texture_elem;
		texture_elem = ft_memalloc(sizeof(t_texture_elem));

		texture_elem->menu = ft_memalloc(sizeof(t_ui_element));
		ui_menu_new(editor->win_main, texture_elem->menu);
		ui_element_set_parent(texture_elem->menu, editor->texture_menu, UI_TYPE_ELEMENT);
		ui_element_edit(texture_elem->menu, texture_menu_button_recipe);
		ui_element_set_id(texture_elem->menu, "texture_button_menu");
		((t_ui_menu *)texture_elem->menu->element)->event_children = 1;
		((t_ui_menu *)texture_elem->menu->element)->render_children = 1;

		int	x = 10;
		int y = 35;
		int	amount_x = editor->texture_menu->pos.w / (texture_elem->menu->pos.w + 5 + x);
		ui_element_pos_set2(texture_elem->menu,
			vec2(x + (i % (amount_x + 1)) * (texture_elem->menu->pos.w + 5),
				y + (i / (amount_x + 1)) * (texture_elem->menu->pos.h + 5)));

		texture_elem->image = ft_memalloc(sizeof(t_ui_element));
		ui_menu_new(editor->win_main, texture_elem->image);
		ui_element_set_parent(texture_elem->image, texture_elem->menu, UI_TYPE_ELEMENT);
		ui_element_edit(texture_elem->image, texture_image_recipe);
		ui_element_set_id(texture_elem->image, "texture_image");
		ui_element_image_set(texture_elem->image, UI_STATE_AMOUNT, editor->wall_textures[i]);

		texture_elem->button = ft_memalloc(sizeof(t_ui_element));
		ui_button_new(editor->win_main, texture_elem->button);
		ui_element_set_parent(texture_elem->button, texture_elem->menu, UI_TYPE_ELEMENT);
		ui_element_edit(texture_elem->button, texture_button_recipe);
		ui_element_set_id(texture_elem->button, "texture_button");

		texture_elem->id = i;

		add_to_list(&editor->texture_elems, texture_elem, sizeof(t_texture_elem));
		add_to_list(&editor->texture_buttons, texture_elem->button, sizeof(t_ui_element));

		ft_printf("[%s] Texture elem #%d made.\n", __FUNCTION__, i);
	}

	// Entity Edit
	editor->entity_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "entity_edit_menu");
	editor->close_entity_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_entity_edit_button");
	editor->entity_texture_surfaces[0] = ui_surface_image_new("ui_images/damage.png");
	editor->entity_textures[0] = ui_texture_create_from_path(editor->win_main->renderer, "ui_images/damage.png");
	for (int i = 1; i <= ENTITY_AMOUNT; i++)
	{
		t_vec4i	pos;
		ft_printf("Load Image : %s\n", g_entity_data[i - 1].path);
		SDL_Surface	*surface = ui_surface_image_new(g_entity_data[i - 1].path);
		editor->entity_texture_surfaces[i] = ui_surface_new(10, 10);
		pos.x = g_entity_data[i - 1].tc[0];
		pos.y = g_entity_data[i - 1].tc[1];
		pos.w = g_entity_data[i - 1].tc[2];
		pos.h = g_entity_data[i - 1].tc[3];
		SDL_BlitScaled(surface, &(SDL_Rect){pos.x, pos.y, pos.w, pos.h}, editor->entity_texture_surfaces[i], NULL);
		editor->entity_textures[i] = SDL_CreateTextureFromSurface(editor->win_main->renderer, editor->entity_texture_surfaces[i]);
		SDL_FreeSurface(surface);
	}
	editor->entity_dropdown = ui_list_get_element_by_id(editor->layout.elements, "entity_dropdown");
	editor->entity_image = ui_list_get_element_by_id(editor->layout.elements, "entity_image");
	editor->entity_z_input = ui_list_get_element_by_id(editor->layout.elements, "entity_z_input");
	editor->entity_yaw_input = ui_list_get_element_by_id(editor->layout.elements, "entity_yaw_input");
	editor->entity_yaw_slider = ui_list_get_element_by_id(editor->layout.elements, "entity_yaw_slider");

	editor->sector_info_label = ui_list_get_element_by_id(editor->layout.elements, "selected_sector_info");
	ui_label_get_label(editor->sector_info_label)->max_w = editor->sector_info_label->pos.w;
	editor->mouse_info_label = ui_list_get_element_by_id(editor->layout.elements, "mouse_hover_info");
	editor->sub_info_label = ui_list_get_element_by_id(editor->layout.elements, "selected_sub_info");
	ui_label_get_label(editor->sub_info_label)->max_w = editor->sub_info_label->pos.w;
	editor->sprite_info_label = ui_list_get_element_by_id(editor->layout.elements, "selected_sprite_info");
	ui_label_get_label(editor->sprite_info_label)->max_w = editor->sprite_info_label->pos.w;
	editor->misc_info_label = ui_list_get_element_by_id(editor->layout.elements, "misc_info");
	ui_label_get_label(editor->misc_info_label)->max_w = editor->misc_info_label->pos.w;

	// Global Info (used for telling user you have saved map for example)
	editor->info_label = ui_list_get_element_by_id(editor->layout.elements, "info_label");
	editor->info_label->show = 0;

	// Sector Hover Info
	editor->sector_hover_info_menu = ui_list_get_element_by_id(editor->layout.elements, "sector_hover_info_menu");
	editor->sector_hover_info_label = ui_list_get_element_by_id(editor->layout.elements, "sector_hover_info_label");

	// Event edit
	editor->event_scrollbar = ui_list_get_element_by_id(editor->layout.elements, "event_scrollbar");
	editor->event_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "event_edit_menu");
	editor->add_event_button = ui_list_get_element_by_id(editor->layout.elements, "add_event_button");
	editor->remove_event_button = ui_list_get_element_by_id(editor->layout.elements, "remove_event_button");
	editor->event_type_dropdown = ui_list_get_element_by_id(editor->layout.elements, "event_type_dropdown");
	editor->event_action_dropdown = ui_list_get_element_by_id(editor->layout.elements, "event_action_dropdown");
	editor->event_id_dropdown = ui_list_get_element_by_id(editor->layout.elements, "event_id_dropdown");
	editor->event_id_menu = ui_dropdown_get_menu_element(editor->event_id_dropdown);
	editor->event_sector_input = ui_list_get_element_by_id(editor->layout.elements, "event_sector_input");
	editor->event_min_input = ui_list_get_element_by_id(editor->layout.elements, "event_min_input");
	editor->event_max_input = ui_list_get_element_by_id(editor->layout.elements, "event_max_input");
	editor->event_speed_input = ui_list_get_element_by_id(editor->layout.elements, "event_speed_input");
	editor->event_menu = ui_list_get_element_by_id(editor->layout.elements, "event_menu");

	ui_element_print(editor->event_sector_input);
	ui_element_print(editor->event_min_input);
	// types
	editor->event_type_floor = ui_list_get_element_by_id(editor->layout.elements, "event_type_floor");
	editor->event_type_ceiling = ui_list_get_element_by_id(editor->layout.elements, "event_type_ceiling");
	editor->event_type_light = ui_list_get_element_by_id(editor->layout.elements, "event_type_light");
	editor->event_type_store = ui_list_get_element_by_id(editor->layout.elements, "event_type_store");
	editor->event_type_hazard = ui_list_get_element_by_id(editor->layout.elements, "event_type_hazard");
	editor->event_type_audio = ui_list_get_element_by_id(editor->layout.elements, "event_type_audio");
	editor->event_type_spawn = ui_list_get_element_by_id(editor->layout.elements, "event_type_spawn");
	// actions
	editor->event_action_click = ui_list_get_element_by_id(editor->layout.elements, "event_action_click");
	editor->event_action_shoot = ui_list_get_element_by_id(editor->layout.elements, "event_action_shoot");
	editor->event_action_sector = ui_list_get_element_by_id(editor->layout.elements, "event_action_sector");
	editor->event_action_null = ui_list_get_element_by_id(editor->layout.elements, "event_action_null");

	// Spawn Edit
	editor->spawn_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "spawn_edit_menu");
	editor->spawn_yaw_input = ui_list_get_element_by_id(editor->layout.elements, "spawn_yaw_input");

	// Save Window
	editor->win_save = ui_list_get_window_by_id(editor->layout.windows, "win_save");
	editor->endless_checkbox = ui_list_get_element_by_id(editor->layout.elements, "endless_checkbox");
	editor->story_checkbox = ui_list_get_element_by_id(editor->layout.elements, "story_checkbox");
	editor->name_input = ui_list_get_element_by_id(editor->layout.elements, "name_input");
	editor->confirm_save_button = ui_list_get_element_by_id(editor->layout.elements, "confirm_save_button");

	// Edit Window
	editor->win_edit = ui_list_get_window_by_id(editor->layout.windows, "win_edit");
	editor->map_scale_input = ui_list_get_element_by_id(editor->layout.elements, "map_scale_input");
	editor->map_scale = 1.0f;

	editor->font = TTF_OpenFont(UI_PATH"fonts/DroidSans.ttf", 20);

	editor->map_type = 0; // endless = 0, story = 1;
	editor->map_name = ft_strdup("map_name.dnd");
	ft_printf("[%s] %s\n", __FUNCTION__, SDL_GetError());
}

void	draw_init(t_editor *editor)
{
	editor->drawing_surface = ui_surface_new(1920, 1030);
	editor->drawing_texture = SDL_CreateTextureFromSurface(editor->win_main->renderer, editor->drawing_surface);
	editor->gap_size = 10.0f;
	editor->zoom = 1.0f;
	editor->offset = vec2i(0, 0);
	editor->grid_surface = ui_surface_new(editor->drawing_surface->w, editor->drawing_surface->h);
	editor->update_grid = 1;
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
		get_map(&editor, editor.map_full_path);
	else
		ft_printf("[%s] No map given.\n", __FUNCTION__);

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
			user_events(&editor, e);
			if (e.key.keysym.scancode == SDL_SCANCODE_P)
			{
				ft_printf("(MOUSE)");
				print_veci(editor.win_main->mouse_pos.v, 2);
				send_info_message(&editor, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
			}
		}
		user_render(&editor);
		ui_layout_render(&editor.layout);
	}
}
