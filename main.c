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

t_wall	*check_if_sector_has_same_kind_of_wall(t_sector *sector, t_wall *wall)
{
	t_list	*curr;
	t_wall	*curr_wall;

	curr = sector->walls;
	while (curr)
	{
		curr_wall = curr->content;
		if ((compare_veci(curr_wall->p1->pos.v, wall->p1->pos.v, 2)
			&& compare_veci(curr_wall->p2->pos.v, wall->p2->pos.v, 2))
			|| (compare_veci(curr_wall->p1->pos.v, wall->p2->pos.v, 2)
			&& compare_veci(curr_wall->p2->pos.v, wall->p1->pos.v, 2)))
			return (curr_wall);
		curr = curr->next;
	}
	return (NULL);
}

int	check_if_you_can_make_wall_portal(t_editor *editor)
{
	t_list		*curr;
	t_sector	*sector;
	t_wall		*wall;

	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		if (sector != editor->selected_sector)
		{
			wall = check_if_sector_has_same_kind_of_wall(sector, editor->selected_wall);
			if (wall)
			{
				editor->selected_wall->neighbor = sector;
				wall->neighbor = editor->selected_sector;
				return (1);
			}
		}
		curr = curr->next;
	}
	return (0);
}

/*
 * 1. Remove all the points not a part of a sector;
 * 2. Remove all walls with either points NULL;
 * 2.5. Remove all neighbors not connected; (if a wall has neighbor but it's corresponding neighbor/sector doesn't have same wall as neighbor, or at all);
 * 3. Remove all sectors with no walls;
*/
void	sector_cleanup(t_editor *editor)
{
	(void)editor;
}

void	sector_events(t_editor *editor, SDL_Event e)
{
	t_vec2i	actual_pos;
	t_vec2i	move_amount;
	char	temp_str[20];

	ft_strnclr(temp_str, 20);

	//calculate_hover(editor); // aleady done;
	actual_pos.x = editor->mouse_pos.x + editor->offset.x;
	actual_pos.y = editor->mouse_pos.y + editor->offset.y;
	move_amount.x = editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x;
	move_amount.y = editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y;
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (!vec2_in_vec4(editor->win_main->mouse_pos, editor->menu_toolbox_top->screen_pos)
			&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
		{
			if (!editor->selected_sector)
			{
				editor->selected_sector = sector_new();
				editor->selected_sector->id = ++editor->sector_amount;
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
					wall = wall_new();
					wall->wall_texture = 1;
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
	else if (editor->select_button->state == UI_STATE_CLICK)
	{
		t_sector	*sector;

		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
		{
			// We dont want to overwrite the currently selected sector with NULL if we dont find sector on mouseclick pos;
			sector = get_sector_from_list_around_radius(editor->sectors, actual_pos, 1);
			if (sector) // this is the place where everything happens when you select new sector;
			{
				editor->selected_sector = sector;
				editor->selected_wall = NULL;
				editor->selected_point = NULL;

				// update sector ui;
				ui_input_set_text(editor->floor_height_input, ft_b_itoa(editor->selected_sector->floor_height, temp_str));
				ui_input_set_text(editor->ceiling_height_input, ft_b_itoa(editor->selected_sector->ceiling_height, temp_str));
				ui_input_set_text(editor->gravity_input, ft_b_itoa(editor->selected_sector->gravity, temp_str));
				ui_input_set_text(editor->lighting_input, ft_b_itoa(editor->selected_sector->lighting, temp_str));
				ui_input_set_text(editor->floor_texture_scale_input, ft_b_ftoa(editor->selected_sector->floor_scale, 2, temp_str));
				ui_input_set_text(editor->ceiling_texture_scale_input, ft_b_ftoa(editor->selected_sector->ceiling_scale, 2, temp_str));
				ft_printf("sector inputs updated\n");
			}
		}
		else if (editor->selected_sector // MOVE SECTOR
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

		// Remove
		if (editor->remove_button->state == UI_STATE_CLICK)
		{
			int	was_removed = 0;
			if (editor->selected_point)
			{
				remove_point(editor, editor->selected_point);
				editor->selected_point = NULL;
			}
			else if (editor->selected_wall)
			{
				remove_wall(editor, editor->selected_wall);
				editor->selected_point = NULL;
			}
			else if (editor->selected_sector)
			{
				remove_sector(editor, editor->selected_sector);
				editor->selected_sector = NULL;
			}
			if (was_removed)
				sector_cleanup(editor);
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
				else if (editor->selected_point && editor->win_main->mouse_down == SDL_BUTTON_RIGHT) // MOVE POINT
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
					// we dont want to make selected_wall NULL if we dont select a wall;
					wall = get_wall_from_list_around_radius(editor->selected_sector->walls, actual_pos, 1.0f);
					if (wall) // this is where we go when new wall selected;
					{
						editor->selected_wall = wall;

						// update wall ui;

						// TODO: ui_checkbox really needs an update, i dont think this is the way to change the toggle state of the checkbox; (it is the way, but shouldnt);
						editor->solid_checkbox->is_click = editor->selected_wall->solid;
						editor->portal_checkbox->is_click = editor->selected_wall->neighbor != NULL;

						ui_input_set_text(editor->floor_wall_angle_input, ft_b_itoa(editor->selected_wall->floor_angle, temp_str));
						ui_input_set_text(editor->ceiling_wall_angle_input, ft_b_itoa(editor->selected_wall->ceiling_angle, temp_str));
						ui_input_set_text(editor->wall_texture_scale_input, ft_b_ftoa(editor->selected_wall->texture_scale, 2, temp_str));
					}
				}
				else if (editor->selected_wall // MOVE WALL
					&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
					&& !(editor->wall_render->show == 1 && ui_element_is_hover(editor->sprite_edit_menu))) // dont move the wall if we are hovering over the sprite_edit_menu;
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

		if (editor->selected_sector && editor->sector_edit_menu->show)
		{
			int	f_height = ft_atoi(ui_input_get_text(editor->floor_height_input));
			int	c_height = ft_atoi(ui_input_get_text(editor->ceiling_height_input));
			int gravity = ft_atoi(ui_input_get_text(editor->gravity_input));
			int lighting = ft_atoi(ui_input_get_text(editor->lighting_input));

			if (ui_input_exit(editor->floor_height_input))
			{
				f_height = ft_min(f_height, c_height);
				ft_b_itoa(f_height, temp_str);
				ui_input_set_text(editor->floor_height_input, temp_str);
			}
			if (ui_input_exit(editor->ceiling_height_input))
			{
				c_height = ft_max(f_height, c_height);
				ft_b_itoa(c_height, temp_str);
				ui_input_set_text(editor->ceiling_height_input, temp_str);
			}
			editor->selected_sector->floor_height = f_height;
			editor->selected_sector->ceiling_height = c_height;

			if (ui_input_exit(editor->gravity_input))
			{
				gravity = ft_clamp(gravity, 0, 100);
				ft_b_itoa(gravity, temp_str);
				ui_input_set_text(editor->gravity_input, temp_str);
			}
			editor->selected_sector->gravity = gravity;

			if (ui_input_exit(editor->lighting_input))
			{
				lighting = ft_clamp(lighting, 0, 100);
				ft_b_itoa(lighting, temp_str);
				ui_input_set_text(editor->lighting_input, temp_str);
			}
			editor->selected_sector->lighting = lighting;

			float f_scale = ft_atof(ui_input_get_text(editor->floor_texture_scale_input));
			float c_scale = ft_atof(ui_input_get_text(editor->ceiling_texture_scale_input));

			if (ui_input_exit(editor->floor_texture_scale_input))
			{
				f_scale = ft_fclamp(f_scale, 0.1f, 100.0f);
				ft_b_ftoa(f_scale, 2, temp_str);
				ui_input_set_text(editor->floor_texture_scale_input, temp_str);
			}
			if (ui_input_exit(editor->ceiling_texture_scale_input))
			{
				c_scale = ft_fclamp(c_scale, 0.1f, 100.0f);
				ft_b_ftoa(c_scale, 2, temp_str);
				ui_input_set_text(editor->ceiling_texture_scale_input, temp_str);
			}
			editor->selected_sector->floor_scale = f_scale;
			editor->selected_sector->ceiling_scale = c_scale;
		}

		// Sector Edit Ok Button
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
		}

		// Sector Edit Menu
		if (editor->sector_button->state == UI_STATE_CLICK && editor->selected_sector)
			editor->sector_edit_menu->show = 1;
		else
			editor->sector_edit_menu->show = 0;

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

		if (editor->selected_sector && editor->selected_wall)
		{
			if (editor->split_wall_button->state == UI_STATE_CLICK)
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

				new_wall = wall_new();
				new_wall->p1 = p1;
				new_wall->p2 = p3;
				add_to_list(&editor->selected_sector->walls, new_wall, sizeof(t_wall));
				add_to_list(&editor->walls, new_wall, sizeof(t_wall));
			}
			// TODO: ui_checkbox really needs an updated, we dont want to update these everytime, we want someway to only do stuff if the toggle state has changed;
			editor->selected_wall->solid = editor->solid_checkbox->state == UI_STATE_CLICK;
			
			// TODO : this needs rework;
			if (editor->portal_checkbox->state == UI_STATE_CLICK)
				if (!check_if_you_can_make_wall_portal(editor))
					editor->portal_checkbox->state = UI_STATE_DEFAULT;

			if (ui_input_exit(editor->floor_wall_angle_input))
			{
				int		angle;
				angle = ft_clamp(ft_atoi(ui_input_get_text(editor->floor_wall_angle_input)), -45, 45);
				ft_b_itoa(angle, temp_str);
				ui_input_set_text(editor->floor_wall_angle_input, temp_str);
				editor->selected_wall->floor_angle = angle;
			}

			if (ui_input_exit(editor->ceiling_wall_angle_input))
			{
				int		angle;
				angle = ft_clamp(ft_atoi(ui_input_get_text(editor->ceiling_wall_angle_input)), -45, 45);
				ft_b_itoa(angle, temp_str);
				ui_input_set_text(editor->ceiling_wall_angle_input, temp_str);
				editor->selected_wall->ceiling_angle = angle;
			}

			if (ui_input_exit(editor->wall_texture_scale_input))
			{
				float	scale;
				scale = ft_fclamp(ft_atof(ui_input_get_text(editor->wall_texture_scale_input)), -10.0f, 10.0f);
				ft_b_ftoa(scale, 2, temp_str);
				ui_input_set_text(editor->wall_texture_scale_input, temp_str);
				editor->selected_wall->texture_scale = scale;
			}
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
	}
}

void	entity_events(t_editor *editor, SDL_Event e)
{
	t_vec2i	actual_pos;
	t_vec2i	move_amount;
	char	temp_str[20];

	ft_strnclr(temp_str, 20);

//	calculate_hover(editor); // already calculated in user_events();
	actual_pos.x = editor->mouse_pos.x + editor->offset.x;
	actual_pos.y = editor->mouse_pos.y + editor->offset.y;
	move_amount.x = editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x;
	move_amount.y = editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y;
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (!vec2_in_vec4(editor->win_main->mouse_pos, editor->menu_toolbox_top->screen_pos)
			&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT) // draw
		{
			t_entity	*entity = entity_new();
			entity->pos = actual_pos;
			++editor->entity_amount;
			add_to_list(&editor->entities, entity, sizeof(t_entity));
		}
	}
	else if (editor->select_button->state == UI_STATE_CLICK)
	{
		if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
		{
			t_entity *entity = get_entity_from_list_around_radius(editor->entities, actual_pos, 1.0f);
			// we dont want to overwrite the currently selected entity if we dont actually find a new one;
			if (entity)
			{
				editor->selected_entity = entity;

				// when selecting new entity this happens;
				ui_input_set_text(editor->entity_yaw_input, ft_b_itoa(editor->selected_entity->yaw, temp_str));
				ui_slider_value_set(editor->entity_yaw_slider, editor->selected_entity->yaw);
			}
		}
		else if (editor->selected_entity
			&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		{
			editor->selected_entity->pos = vec2i_add(editor->selected_entity->pos, move_amount);
		}

		if (editor->selected_entity)
		{
			if (ui_input_exit(editor->entity_yaw_input))
			{
				int	angle = ft_atoi(ui_input_get_text(editor->entity_yaw_input));
				angle = ft_clamp(angle, 0, 360);
				editor->selected_entity->yaw = angle;
				ui_input_set_text(editor->entity_yaw_input, ft_b_itoa(angle, temp_str));
				ui_slider_value_set(editor->entity_yaw_slider, angle);
			}
			if (ui_slider_updated(editor->entity_yaw_slider))
			{
				int	angle = ui_slider_get_slider(editor->entity_yaw_slider)->value;
				editor->selected_entity->yaw = angle;
				ui_input_set_text(editor->entity_yaw_input, ft_b_itoa(angle, temp_str));
			}
			
			if (ui_dropdown_exit(editor->entity_dropdown)
				&& ui_dropdown_get_dropdown(editor->entity_dropdown)->active)
			{
				ft_printf("We are checking for name\n");
				int	i = 0;
				while (++i < ENTITY_AMOUNT + 1) // i know that we are starting at 1, unset is 0;
				{
					ft_printf("%s, ", g_entity_data[i - 1].name);
					ft_printf("%s\n", ui_button_get_label(ui_dropdown_get_dropdown(editor->entity_dropdown)->active)->text);
					if (ft_strequ(g_entity_data[i - 1].name, ui_button_get_label(ui_dropdown_get_dropdown(editor->entity_dropdown)->active)->text))
					{
						editor->selected_entity->type = i;
						ft_printf("entity type : %d selected.\n", editor->selected_entity->type);
						break ;
					}
				}
				// update entity image;
				t_vec4i	pos;
				int type = editor->selected_entity->type - 1;
				if (type > 0)
				{
					SDL_Surface	*surface = ui_surface_image_new(g_entity_data[type].path);
					SDL_Surface *blat = ui_surface_new(10, 10);
					pos.x = g_entity_data[type].tc[0];
					pos.y = g_entity_data[type].tc[1];
					pos.w = g_entity_data[type].tc[2];
					pos.h = g_entity_data[type].tc[3];
					SDL_BlitScaled(surface, &(SDL_Rect){pos.x, pos.y, pos.w, pos.h}, blat, NULL);
					ui_element_image_set(editor->entity_image, UI_STATE_DEFAULT, blat);
					SDL_FreeSurface(surface);
					SDL_FreeSurface(blat);
				}
			}
		}

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

	if (ui_list_radio_event(editor->event_element_buttons, &editor->active_event_elem))
	{
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

		ft_printf("a new event button clicked. lets updated some dropdowns.\n");

		ui_dropdown_activate(editor->event_type_dropdown,
			ui_list_get_button_with_text(ui_dropdown_get_menu_element(editor->event_type_dropdown)->children,
				g_event_type[editor->selected_event_elem->event->type]));
		ui_dropdown_activate(editor->event_action_dropdown,
			ui_list_get_button_with_text(ui_dropdown_get_menu_element(editor->event_action_dropdown)->children,
				g_event_action[editor->selected_event_elem->event->action]));
	}

	if (editor->add_event_button->state == UI_STATE_CLICK)
	{
		t_event_elem	*event_elem;
		if (editor->selected_event_elem == NULL)
		{
			char			*temp2;
			char			temp[20];

			ft_printf("Trying to make new event\n");
			event_elem = event_element_new(editor->win_main, &editor->layout, editor->event_menu);
			event_elem->event = event_new();
			event_elem->nth = editor->event_amount++;

			temp2 = ft_strjoin("event_", ft_b_itoa(event_elem->nth, temp));
			ui_label_text_set(ui_button_get_label_element(event_elem->button), temp2);
			ft_strdel(&temp2);

			t_vec2	new_pos;
			new_pos = vec2(event_elem->menu->pos.x, ((t_ui_scrollbar *)editor->event_scrollbar->element)->bot_most.y + 10);
			ui_element_pos_set2(event_elem->menu, new_pos);
			
			add_to_list(&editor->event_elements, event_elem, sizeof(t_event_elem));
			add_to_list(&editor->event_element_buttons, event_elem->button, sizeof(t_ui_element));
			add_to_list(&editor->events, event_elem->event, sizeof(t_event));
			ft_printf("New event added (%d)\n", editor->event_amount);
			editor->selected_event_elem = event_elem;
			editor->active_event_elem = event_elem->button;
			editor->selected_event = event_elem->event;
		}
		event_elem_update(editor, editor->selected_event_elem);
		editor->selected_event_elem = NULL;
		editor->active_event_elem = NULL;
		editor->selected_event = NULL;
	}

	editor->event_sector_input->show = 0;
	editor->event_min_input->show = 0;
	editor->event_max_input->show = 0;
	editor->event_speed_input->show = 0;
	editor->event_action_dropdown->show = 0;
	editor->event_id_dropdown->show = 0;
	if (ui_dropdown_active(editor->event_type_dropdown))
	{
		editor->event_action_dropdown->show = 1;
		editor->event_id_dropdown->show = 1;
	}

	if (editor->event_type_floor->state == UI_STATE_CLICK)
	{
		editor->event_sector_input->show = 1;
		editor->event_min_input->show = 1;
		editor->event_max_input->show = 1;
		editor->event_speed_input->show = 1;
	}
	else if (editor->event_type_ceiling->state == UI_STATE_CLICK)
	{
		editor->event_sector_input->show = 1;
		editor->event_min_input->show = 1;
		editor->event_max_input->show = 1;
		editor->event_speed_input->show = 1;
	}
	else if (editor->event_type_light->state == UI_STATE_CLICK)
	{
		editor->event_sector_input->show = 1;
		editor->event_min_input->show = 1;
		editor->event_max_input->show = 1;
	}
	else if (editor->event_type_store->state == UI_STATE_CLICK)
	{
	}
	else if (editor->event_type_hazard->state == UI_STATE_CLICK)
	{
		editor->event_speed_input->show = 1;
	}
	else if (editor->event_type_audio->state == UI_STATE_CLICK)
	{
		editor->event_sector_input->show = 1;
	}
	else if (editor->event_type_spawn->state == UI_STATE_CLICK)
	{
		editor->event_sector_input->show = 1;
		editor->event_min_input->show = 1;
		editor->event_max_input->show = 1;
		editor->event_speed_input->show = 1;
	}

	//
	if (editor->event_id_dropdown->show
		&& ui_dropdown_open(editor->event_id_dropdown)
		&& (editor->event_action_shoot->state == UI_STATE_CLICK
			|| editor->event_action_click->state == UI_STATE_CLICK))
	{
		ft_printf("event id dropdown clicked, we want to update now.\n");

		t_list		*curr;
		t_list		*curr_button;
		t_sprite	*sprite;
		int			id;
		char		temp_str[20];

		// Count how many of type action we have in sprites; save in event_id_buttons_in_use;
		// Might aswell update the ids of them;
		id = -1;
		editor->event_id_buttons_in_use = 0;
		curr = editor->sprites;
		while (curr)
		{
			sprite = curr->content;
			if (sprite->type == ACTION)
				editor->event_id_buttons_in_use++;
			sprite->id = ++id;
			curr = curr->next;
		}
		// Create as many buttons as we have sprites with action as type;
		while (editor->event_id_buttons_made < editor->event_id_buttons_in_use)
		{
			t_ui_element	*elem;
			elem = ft_memalloc(sizeof(t_ui_element));
			ui_button_new(editor->win_main, elem);
			ui_element_set_parent(elem, editor->event_id_menu, UI_TYPE_ELEMENT);
			// ui_element_edit(elem, recipe);
			editor->event_id_buttons_made++;
		}
		// Fill all the buttons with the id of all the sprites;
		curr = editor->sprites;
		curr_button = editor->event_id_menu->children;
		while (curr) // curr_button should never be NULL before curr!
		{
			sprite = curr->content;
			((t_ui_element *)curr_button->content)->show = 1;
			if (sprite->type == ACTION)
				ui_button_set_text(curr_button->content, ft_b_itoa(sprite->id, temp_str));
			curr = curr->next;
			curr_button = curr_button->next;
		}
		// If we have more made buttons than buttons in use,
		// 	go through the rest of them and hide them.
		while (curr_button)
		{
			((t_ui_element *)curr_button->content)->show = 0;
			curr_button = curr_button->next;
		}

		ft_printf("Event iD buttons in use : %d, made : %d\n", editor->event_id_buttons_in_use, editor->event_id_buttons_made);
	}
}

void	spawn_events(t_editor *editor, SDL_Event e)
{
	t_vec2i	actual_pos;
	t_vec2i	move_amount;
	char	temp_str[20];

	ft_strnclr(temp_str, 20);

//	calculate_hover(editor); // already calculated in user_events();
	actual_pos.x = editor->mouse_pos.x + editor->offset.x;
	actual_pos.y = editor->mouse_pos.y + editor->offset.y;
	move_amount.x = editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x;
	move_amount.y = editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y;
	if (editor->draw_button->state == UI_STATE_CLICK)
	{
		if (!vec2_in_vec4(editor->win_main->mouse_pos, editor->menu_toolbox_top->screen_pos))
			if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT)
				editor->spawn.pos = actual_pos;
	}
	else if (editor->select_button->state == UI_STATE_CLICK)
	{
		// TODO: show spawn edit menu when select_button clicked;

		if (editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
			editor->spawn.pos = vec2i_add(editor->spawn.pos, move_amount);
	}
}

void	info_menu_events(t_editor *editor, SDL_Event e)
{
	char	*final_str;
	t_vec2i	actual_pos;

	actual_pos.x = editor->mouse_pos.x + editor->offset.x;
	actual_pos.y = editor->mouse_pos.y + editor->offset.y;
	if (e.type == SDL_MOUSEMOTION)
	{
		final_str = ft_sprintf("%d, %d", actual_pos.x, actual_pos.y);
		ui_label_text_set(editor->mouse_info_label, final_str);
		ft_strdel(&final_str);
	}

	// TODO: figure out when to update this;
	final_str = ft_sprintf("points : %d\nwalls : %d\nsectors : %d\nentities : %d\n",
			editor->point_amount, editor->wall_amount, editor->sector_amount, editor->entity_amount);
	ui_label_text_set(editor->misc_info_label, final_str);
	ft_strdel(&final_str);

	if (editor->selected_sector)
	{
		t_sector *sector = editor->selected_sector;
		final_str = ft_sprintf("iD : %d\npos : %d, %d\nWalls : %d\n", sector->id, sector->center.x, sector->center.y, sector->wall_amount);
		ui_label_text_set(editor->sector_info_label, final_str);
		ft_strdel(&final_str);

		if (editor->selected_point)
		{
			t_point *point = editor->selected_point;
			final_str = ft_sprintf("iD : %d\npos : %d, %d\n", point->id, point->pos.x, point->pos.y);
			ui_label_text_set(editor->sub_info_label, final_str);
			ft_strdel(&final_str);
		}
		else if (editor->selected_wall)
		{
			t_wall *wall = editor->selected_wall;
			final_str = ft_sprintf("iD : %d\np1 : %d, %d\np2 : %d, %d\n", wall->id, wall->p1->pos.x, wall->p1->pos.y, wall->p2->pos.x, wall->p2->pos.y);
			ui_label_text_set(editor->sub_info_label, final_str);
			ft_strdel(&final_str);
		}
		else
			ui_label_text_set(editor->sub_info_label, "NONE");
	}
	else
	{
		ui_label_text_set(editor->sector_info_label, "NONE");
		ui_label_text_set(editor->sub_info_label, "NONE");
	}
}

void	save_window_events(t_editor *editor, SDL_Event e)
{
	if (editor->save_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_save, UI_WINDOW_SHOW);// | UI_WINDOW_RAISE);

	if (editor->confirm_save_button->state == UI_STATE_CLICK)
	{
		int	len;

		len = ft_strlen(ui_input_get_text(editor->name_input));
		if (len < 1)
			ft_printf("[%s] Map name len is less than 1 (it\'s : %d), won\'t slide.\n", __FUNCTION__, len);
		else
		{
			char	*actual_full_path;

			actual_full_path = ft_strjoiner(MAP_PATH, ui_input_get_text(editor->name_input), ".dnd", NULL);
			set_map(editor, actual_full_path);
			ft_strdel(&actual_full_path);
		}
	}
}

void	edit_window_events(t_editor *editor, SDL_Event e)
{
	if (!editor->win_edit->show)
		return ;
	if (editor->edit_button->state == UI_STATE_CLICK)
		ui_window_flag_set(editor->win_edit, UI_WINDOW_SHOW);// | UI_WINDOW_RAISE);
	if (ui_input_exit(editor->map_scale_input))
		editor->map_scale = ft_atof(ui_input_get_text(editor->map_scale_input));
}

void	render_wall_on_sprite_menu(t_editor *editor, t_sector *sector, t_wall *wall)
{
	SDL_Surface	*surface;
	t_vec2i		dist;
	float		aspect;

//	ft_printf("wall redner size : %d %d\n", (int)editor->wall_render->pos.w, (int)editor->wall_render->pos.h);

	// TODO: figure out if we even need any of the mapscale here, since if everything gets scaled up... it doesnt look like anything is scaling up;
	dist.x = distancei(wall->p1->pos.v, wall->p2->pos.v, 2);
	dist.y = (sector->ceiling_height - sector->floor_height);
//	ft_printf("distance : %d %d\n", dist.x, dist.y);
	aspect = get_ratio(vec2i(dist.x, dist.y), vec2i(editor->wall_render->pos.w, editor->wall_render->pos.h));
//	ft_printf("aspect2 : %.2f\n", aspect);
	surface = ui_surface_new(dist.x * aspect, dist.y * aspect);
//	ft_printf("size : %d %d\n", surface->w, surface->h);

	ui_texture_fill(editor->win_main->renderer, editor->wall_render->texture, 0xff000000);

	// Render the Texture
	SDL_Surface	*texture = editor->wall_textures[wall->wall_texture];
	t_vec2i		from = vec2i(ft_min(texture->w, texture->h), ft_min(texture->w, texture->h));
	t_vec2i		scaled_size = vec2i(64 * wall->texture_scale, 64 * wall->texture_scale);
	t_vec2i		dim = vec2i(ft_min(scaled_size.x, scaled_size.y), ft_min(scaled_size.x, scaled_size.y));
	int			xtexturecount = surface->w / dim.x;
	int			ytexturecount = surface->h / dim.y;
	for (int j = 0; j <= ytexturecount; j++)
		for (int i = 0; i <= xtexturecount; i++)
			SDL_BlitScaled(texture, &(SDL_Rect){0, 0, from.x, from.y}, surface, &(SDL_Rect){i * dim.x, j * dim.y, dim.x, dim.y});

	// Render the sprites;
	t_sprite	*sprite;
	t_list		*curr;

	curr = wall->sprites;
	while (curr)
	{
		sprite = curr->content;
		texture = editor->wall_textures[sprite->texture];
		from = vec2i(ft_min(texture->w, texture->h), ft_min(texture->w, texture->h));
		sprite->pos.w = 64 * sprite->scale;
		sprite->pos.h = 64 * sprite->scale;
		dim = vec2i(ft_min(scaled_size.x, scaled_size.y), ft_min(scaled_size.x, scaled_size.y));
		SDL_BlitScaled(texture, &(SDL_Rect){0, 0, from.x, from.y}, surface, &(SDL_Rect){sprite->pos.x, sprite->pos.y, dim.x, dim.y});
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

	SDL_UpdateTexture(editor->wall_render->texture, &(SDL_Rect){0, 0, surface->w, surface->h}, surface->pixels, surface->pitch);
	SDL_FreeSurface(surface);
}

void	sprite_events(t_editor *editor, SDL_Event e)
{
	if (!editor->selected_wall || !editor->selected_sector)
	{
		ft_printf("[%s] We need both selected wall and sector for this function, and currently we dont have both so no function running for you.\n");
		return ;
	}

	editor->sprite_edit_menu->show = 1;

	if (editor->sprite_add_button->state == UI_STATE_CLICK)
	{
		editor->selected_sprite = sprite_new();
		editor->selected_sprite->pos = vec4i(0, 0, 64, 64);
		editor->selected_sprite->texture = 0;
		editor->selected_sprite->scale = 1.0f;
		editor->selected_sprite->type = ACTION;
		add_to_list(&editor->selected_wall->sprites, editor->selected_sprite, sizeof(t_sprite));
		add_to_list(&editor->sprites, editor->selected_sprite, sizeof(t_sprite));
		ft_printf("New Sprite Added (%d)\n", ++editor->selected_wall->sprite_amount);
	}
	render_wall_on_sprite_menu(editor, editor->selected_sector, editor->selected_wall);

	// Selecting Sprite
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& ui_element_is_hover(editor->wall_render))
	{
		t_list		*curr;
		t_sprite	*sprite;
		t_vec2i		actual_mouse_pos;

		actual_mouse_pos = vec2i(editor->win_main->mouse_pos.x - editor->wall_render->screen_pos.x,
				editor->win_main->mouse_pos.y - editor->wall_render->screen_pos.y);
		curr = editor->selected_wall->sprites;
		while (curr)
		{
			sprite = curr->content;
			if (vec2_in_vec4(actual_mouse_pos, sprite->pos))
			{
				ft_printf("We have clicked a sprite.\n");
				editor->selected_sprite = sprite;
				break ;
			}
			curr = curr->next;
		}
	}

	// Moving Sprite
	if (editor->selected_sprite
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
	{
		t_vec2i	move_amount;

		move_amount.x = editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x;
		move_amount.y = editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y;
		editor->selected_sprite->pos.x = editor->selected_sprite->pos.x + move_amount.x;
		editor->selected_sprite->pos.y = editor->selected_sprite->pos.y + move_amount.y;
	}
}

void	user_events(t_editor *editor, SDL_Event e)
{
	t_vec2i	actual_pos;
	t_vec2i	move_amount;
	char	temp_str[20];

	ft_strnclr(temp_str, 20);

	calculate_hover(editor);
	actual_pos.x = editor->mouse_pos.x + editor->offset.x;
	actual_pos.y = editor->mouse_pos.y + editor->offset.y;
	move_amount.x = editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x;
	move_amount.y = editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y;

	// Reset Grid Pos
	if (e.key.keysym.scancode == SDL_SCANCODE_SPACE)
		editor->offset = vec2i(0, 0);

	// Moving Grid
	if (editor->win_main->mouse_down == SDL_BUTTON_MIDDLE)
	{
		editor->offset.x -= (editor->win_main->mouse_pos.x - editor->win_main->mouse_pos_prev.x);
		editor->offset.y -= (editor->win_main->mouse_pos.y - editor->win_main->mouse_pos_prev.y);
	}

	if (ui_button(editor->draw_button))
	{
		editor->selected_sector = NULL;
		editor->selected_entity = NULL;
		editor->selected_event = NULL;
	}

	if (editor->sector_button->state == UI_STATE_CLICK)
	{
		sector_events(editor, e);
		if (editor->selected_wall
			&& editor->wall_button->state == UI_STATE_CLICK)
			sprite_events(editor, e);
		else
		{
			editor->selected_sprite = NULL;
			editor->sprite_edit_menu->show = 0;
		}
	}
	else
	{
		editor->selected_sector = NULL;
		editor->sector_edit_menu->show = 0;
	}

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
		spawn_events(editor, e);
	else
	{
		//editor->selected_spawn = NULL;
		//editor->spawn_edit_menu->show = 0;
	}

	info_menu_events(editor, e);
	save_window_events(editor, e);
	edit_window_events(editor, e);
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
		if (wall->neighbor)
		{
			if (check_if_sector_has_same_kind_of_wall(wall->neighbor, wall))
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
	while (entities)
	{
		entity_yaw_render(editor, entities->content);
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
}

void	user_render(t_editor *editor)
{
	draw_grid(editor->drawing_surface, editor->gap_size, editor->zoom);
	draw_sectors(editor, editor->sectors);
	draw_hover(editor);
	draw_selected(editor);

	draw_entities_yaw(editor, editor->entities);
	draw_spawn(editor);

	SDL_UpdateTexture(editor->drawing_texture, NULL, editor->drawing_surface->pixels, editor->drawing_surface->pitch);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer, editor->drawing_texture, NULL, NULL);

	draw_entities(editor, editor->entities);

	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
	SDL_FillRect(editor->drawing_surface, NULL, 0);

}

void	editor_init(t_editor *editor)
{
	memset(editor, 0, sizeof(t_editor));
	ui_layout_load(&editor->layout, "layout.ui");

	// Main Window
	editor->win_main = ui_list_get_window_by_id(editor->layout.windows, "win_main");

	// Selection Menu
	editor->menu_toolbox_top = ui_list_get_element_by_id(editor->layout.elements, "menu_toolbox_top");
	editor->menu_selection = ui_list_get_element_by_id(editor->layout.elements, "menu_select_buttons");
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
	// Inputs;
	editor->floor_height_input = ui_list_get_element_by_id(editor->layout.elements, "floor_height_input");
	editor->ceiling_height_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_height_input");
	editor->gravity_input = ui_list_get_element_by_id(editor->layout.elements, "gravity_input");
	editor->lighting_input = ui_list_get_element_by_id(editor->layout.elements, "lighting_input");
	editor->floor_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "floor_texture_scale_input");
	editor->ceiling_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_texture_scale_input");

	// Wall
	editor->menu_wall_edit = ui_list_get_element_by_id(editor->layout.elements, "menu_wall_edit");
	editor->close_wall_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_wall_edit_button");
	editor->solid_checkbox = ui_list_get_element_by_id(editor->layout.elements, "solidity_checkbox");
	editor->portal_checkbox = ui_list_get_element_by_id(editor->layout.elements, "portal_checkbox");
	editor->split_wall_button = ui_list_get_element_by_id(editor->layout.elements, "split_wall_button");
	editor->wall_texture_button = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_button");
	editor->portal_texture_button = ui_list_get_element_by_id(editor->layout.elements, "portal_texture_button");
	editor->floor_wall_angle_input = ui_list_get_element_by_id(editor->layout.elements, "floor_wall_angle_input");
	editor->ceiling_wall_angle_input = ui_list_get_element_by_id(editor->layout.elements, "ceiling_wall_angle_input");
	editor->wall_texture_scale_input = ui_list_get_element_by_id(editor->layout.elements, "wall_texture_scale_input");

	editor->texture_menu = ui_list_get_element_by_id(editor->layout.elements, "texture_menu");
	editor->texture_menu->show = 0;
	editor->texture_menu_close_button = ui_list_get_element_by_id(editor->layout.elements, "texture_menu_close_button");

	// Sprite
	editor->sprite_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "sprite_edit_menu");
	editor->sprite_add_button = ui_list_get_element_by_id(editor->layout.elements, "sprite_add_button");
	editor->sprite_remove_button = ui_list_get_element_by_id(editor->layout.elements, "sprite_remove_button");
	editor->wall_render = ui_list_get_element_by_id(editor->layout.elements, "wall_render");
	ui_element_textures_redo(editor->wall_render); // its dumb that i have to do this;
	for (int i = 0; i < MAP_TEXTURE_AMOUNT; i++)
	{
		ft_printf("Load Image : %s\n", g_map_textures[i].path);
		editor->wall_textures[i] = load_bxpm_to_surface(g_map_textures[i].path);
	}

	// Entity Edit
	editor->entity_edit_menu = ui_list_get_element_by_id(editor->layout.elements, "entity_edit_menu");
	editor->close_entity_edit_button = ui_list_get_element_by_id(editor->layout.elements, "close_entity_edit_button");
	editor->entity_textures[0] = ui_texture_create_from_path(editor->win_main->renderer, "ui_images/damage.png");
	for (int i = 1; i < ENTITY_AMOUNT + 1; i++)
	{
		t_vec4i	pos;
		ft_printf("Load Image : %s\n", g_entity_data[i - 1].path);
		SDL_Surface	*surface = ui_surface_image_new(g_entity_data[i - 1].path);
		SDL_Surface *blat = ui_surface_new(10, 10);
		pos.x = g_entity_data[i - 1].tc[0];
		pos.y = g_entity_data[i - 1].tc[1];
		pos.w = g_entity_data[i - 1].tc[2];
		pos.h = g_entity_data[i - 1].tc[3];
		SDL_BlitScaled(surface, &(SDL_Rect){pos.x, pos.y, pos.w, pos.h}, blat, NULL);
		editor->entity_textures[i] = SDL_CreateTextureFromSurface(editor->win_main->renderer, blat);
		SDL_FreeSurface(surface);
		SDL_FreeSurface(blat);
	//	editor->entity_textures[i] = ui_texture_create_from_path(editor->win_main->renderer, g_entity_data[i - 1].path);
	}
	editor->entity_dropdown = ui_list_get_element_by_id(editor->layout.elements, "entity_dropdown");
	editor->entity_image = ui_list_get_element_by_id(editor->layout.elements, "entity_image");
	editor->entity_yaw_input = ui_list_get_element_by_id(editor->layout.elements, "entity_yaw_input");
	editor->entity_yaw_slider = ui_list_get_element_by_id(editor->layout.elements, "entity_yaw_slider");

	editor->sector_info_label = ui_list_get_element_by_id(editor->layout.elements, "selected_sector_info");
	ui_label_get_label(editor->sector_info_label)->max_w = editor->sector_info_label->pos.w;
	editor->mouse_info_label = ui_list_get_element_by_id(editor->layout.elements, "mouse_hover_info");
	editor->sub_info_label = ui_list_get_element_by_id(editor->layout.elements, "selected_sub_info");
	ui_label_get_label(editor->sub_info_label)->max_w = editor->sub_info_label->pos.w;
	editor->misc_info_label = ui_list_get_element_by_id(editor->layout.elements, "misc_info");
	ui_label_get_label(editor->misc_info_label)->max_w = editor->misc_info_label->pos.w;

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

	// Save Window
	editor->win_save = ui_list_get_window_by_id(editor->layout.windows, "win_save");
	editor->endless_button = ui_list_get_element_by_id(editor->layout.elements, "endless_button");
	editor->story_button = ui_list_get_element_by_id(editor->layout.elements, "story_button");
	editor->name_input = ui_list_get_element_by_id(editor->layout.elements, "name_input");
	editor->confirm_save_button = ui_list_get_element_by_id(editor->layout.elements, "confirm_save_button");

	// Edit Window
	editor->win_edit = ui_list_get_window_by_id(editor->layout.windows, "win_edit");
	editor->map_scale_input = ui_list_get_element_by_id(editor->layout.elements, "map_scale_input");
	editor->map_scale = 1.0f;

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
				t_ui_element *b = ui_list_get_element_by_id(editor.layout.elements, "event_0");
				ui_element_print(b);
				ui_element_print(ui_button_get_label_element(b));
			}
		}
		// User Render
		user_render(&editor);
		
		ui_layout_render(&editor.layout);
	}
}
