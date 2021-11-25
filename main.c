#include "editor.h"

/*
 * From in game position calculate the screen position;
*/
void	draw_hover(t_editor *editor)
{
	ui_surface_circle_draw_filled(editor->drawing_surface,
		editor->mouse_pos_screen, 3, 0xffffffff);
	ui_surface_line_draw(editor->drawing_surface,
		vec2i(editor->mouse_pos_screen.x, 0),
		vec2i(editor->mouse_pos_screen.x, editor->drawing_surface->h),
		0x70b0b0b0);
	ui_surface_line_draw(editor->drawing_surface,
		vec2i(0, editor->mouse_pos_screen.y),
		vec2i(editor->drawing_surface->w, editor->mouse_pos_screen.y),
		0x70b0b0b0);
	if (editor->first_point_set)
		ui_surface_line_draw(editor->drawing_surface,
			conversion(editor, editor->first_point),
			editor->mouse_pos_screen, 0xffffff00);
}

bool	can_you_make_portal_of_this_wall(
		t_list *sector_list, t_sector *part_of_sector, t_wall *wall)
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
			temp_wall = get_sector_wall_at_pos(sector,
					wall->p1->pos, wall->p2->pos);
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

void	send_info_message(t_editor *editor, char *text)
{
	editor->info_label_start_timer = SDL_GetTicks();
	editor->info_label_timer = 0;
	ui_label_color_set(editor->info_label, 0xffffffff);
	ui_label_set_text(editor->info_label, text);
	editor->info_label->show = 1;
}

void	draw_grid_lines(SDL_Surface *surface, float gap_size, float zoom)
{
	int			w_amount;
	int			h_amount;
	int			i;

	w_amount = surface->w / (gap_size * zoom);
	h_amount = surface->h / (gap_size * zoom);
	i = 0;
	while (i < w_amount || i < h_amount)
	{
		if (i < w_amount)
			ui_surface_line_draw(surface, vec2i(i * (gap_size * zoom), 0),
				vec2i(i * (gap_size * zoom), surface->h), 0xff353535);
		if (i < h_amount)
			ui_surface_line_draw(surface, vec2i(0, i * (gap_size * zoom)),
				vec2i(surface->w, i * (gap_size * zoom)), 0xff353535);
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
			if (get_sector_wall_at_pos(wall->neighbor,
					wall->p1->pos, wall->p2->pos))
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

void	sector_check_errors(t_editor *editor, t_sector *sector)
{
	if (!check_sector_convexity(sector))
	{
		draw_text(editor->drawing_surface, "Not Convex!",
			editor->font, sector->screen_center, 0xffff0000); 
		editor->errors += 1;
	}
	if (sector->ceiling_height - sector->floor_height < 0)
	{
		draw_text(editor->drawing_surface,
			"Floor & Ceiling Height Doesn\'t Make Sense!",
			editor->font, sector->screen_center, 0xffffff00); 
		editor->errors += 1;
	}
}

void	draw_sectors(t_editor *editor, t_list *sectors)
{
	t_sector	*sector;
	char		temp[20];
	t_sector	*hovered_this_frame;

	hovered_this_frame = NULL;
	while (sectors)
	{
		sector = sectors->content;
		sort_walls(sector->walls); // TODO do this everytime the sector has been updated, not every frame (if its slow);
		sector_render(editor, sector, sector->color);
		sector->center = get_sector_center(sector);
		sector->screen_center = conversion(editor, sector->center);
		ft_b_itoa(sector->id, temp);
		draw_text(editor->drawing_surface,
			temp, editor->font, sector->screen_center, 0xffffffff);
		sector_check_errors(editor, sector);
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

void	entity_inside_which_sector(t_list *sectors, t_entity *entity)
{
	t_list	*curr;

	curr = sectors;
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
}

void	entity_check_errors(t_editor *editor, t_entity *entity)
{
	if (!entity->inside_sector)
	{
		draw_text(editor->drawing_surface, "Not Inside Sector!",
			editor->font, conversion(editor, entity->pos), 0xffff0000); 
		editor->errors += 1;
	}
	else if (entity->inside_sector
		&& (entity->z < entity->inside_sector->floor_height
			|| entity->z > entity->inside_sector->ceiling_height))
	{
		draw_text(editor->drawing_surface, "Z not between Floor & Ceiling!",
			editor->font, conversion(editor, entity->pos), 0xffff0000); 
		editor->errors += 1;
	}
}

void	draw_entities_yaw(t_editor *editor, t_list *entities)
{
	t_entity	*entity;

	while (entities)
	{
		entity = entities->content;
		entity_yaw_render(editor, entities->content);
		entity_inside_which_sector(editor->sectors, entity);
		entity_check_errors(editor, entity);
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
	t_list	*curr;

	ui_surface_circle_draw_filled(editor->drawing_surface,
		conversion(editor, editor->spawn.pos), 10, 0xff00ff00);
	curr = editor->sectors;
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

void	draw_grid(t_editor *editor)
{
	if (editor->update_grid)
	{
		draw_grid_lines(editor->grid_surface, editor->gap_size, editor->zoom);
		editor->update_grid = 0;
		ft_printf("[%s] We are updating grid surface.\n", __FUNCTION__);
	}
	SDL_BlitSurface(editor->grid_surface, NULL, editor->drawing_surface, NULL);
}

void	update_error_label(t_editor *editor)
{
	char	*temp;

	if (editor->errors > 0)
	{
		temp = ft_sprintf("Consider fixing the %d error(s) before saving!",
				editor->errors);
		ui_label_set_text(editor->error_label, temp);
		ft_strdel(&temp);
		editor->error_label->show = 1;
	}
	else
		editor->error_label->show = 0;
}

void	user_render(t_editor *editor)
{
	editor->errors = 0;
	draw_grid(editor);
	draw_sectors(editor, editor->sectors);
	draw_hover(editor);
	draw_selected(editor);
	draw_entities_yaw(editor, editor->entities);
	draw_spawn(editor);
	SDL_UpdateTexture(editor->drawing_texture, NULL,
		editor->drawing_surface->pixels, editor->drawing_surface->pitch);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer,
		editor->drawing_texture, NULL, NULL);
	draw_entities(editor, editor->entities);
	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
	SDL_FillRect(editor->drawing_surface, NULL, 0);
	update_error_label(editor);
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
	editor->drawing_texture
			= SDL_CreateTextureFromSurface(editor->win_main->renderer,
				editor->drawing_surface);
	editor->gap_size = 10.0f;
	editor->zoom = 1.0f;
	editor->offset = vec2i(0, 0);
	editor->grid_surface = ui_surface_new(
			editor->drawing_surface->w, editor->drawing_surface->h);
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
	t_fps		fps;
	SDL_Event	e;

	ui_sdl_init();
	editor_init(&editor);
	draw_init(&editor);
	if (args_parser(&editor, ac, av))
		get_map(&editor, editor.map_full_path);
	else
		ft_printf("[%s] No map given.\n", __FUNCTION__);
	memset(&fps, 0, sizeof(t_fps));
	while (!editor.win_main->wants_to_close)
	{
		fps_func(&fps);
		update_title_fps(editor.win_main->win, &fps);
		while (SDL_PollEvent(&e))
		{
			ui_layout_event(&editor.layout, e);
			user_events(&editor, e);
		}
		user_render(&editor);
		ui_layout_render(&editor.layout);
	}
}
