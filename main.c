#include "editor.h"

void	new_id_button(t_ui_element *parent, char *text, t_ui_recipe *recipe)
{
	t_ui_element	*elem;

	elem = ft_memalloc(sizeof(t_ui_element));
	ui_button_new(parent->win, elem);
	ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
	ui_element_edit(elem, recipe);
	ui_button_set_text(elem, text);
}

/*
 * create buttons to 'parent->children' with the texts from 'texts',
 *	all buttons have 'recipe' recipe; Remove all extra buttons in the list;
*/
void	create_buttons_to_list_from_texts_remove_extra(
		t_ui_element *parent, char **texts, t_ui_recipe *recipe)
{
	int				i;
	t_list			*prev_butt;

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
			new_id_button(parent, texts[i], recipe);
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

void	editor_init(t_editor *editor)
{
	memset(editor, 0, sizeof(t_editor));
	ui_layout_load(&editor->layout, EDITOR_PATH"layout.ui");

	// Main Window
	editor->win_main = ui_layout_get_window(&editor->layout, "win_main");

	// Selection Menu
	editor->menu_toolbox_top = ui_layout_get_element(&editor->layout, "menu_toolbox_top");
	editor->menu_selection = ui_layout_get_element(&editor->layout, "menu_select_buttons");
	editor->selection_dropdown_menu = ui_layout_get_element(&editor->layout, "type_drop_menu");
	editor->draw_button = ui_layout_get_element(&editor->layout, "draw_button");
	editor->select_button = ui_layout_get_element(&editor->layout, "select_button");
	editor->remove_button = ui_layout_get_element(&editor->layout, "remove_button");
	editor->point_button = ui_layout_get_element(&editor->layout, "point_button");
	editor->wall_button = ui_layout_get_element(&editor->layout, "wall_button");
	editor->sector_button = ui_layout_get_element(&editor->layout, "sector_button");
	editor->entity_button = ui_layout_get_element(&editor->layout, "entity_button");
	editor->spawn_button = ui_layout_get_element(&editor->layout, "spawn_button");
	editor->event_button = ui_layout_get_element(&editor->layout, "event_button");
	editor->save_button = ui_layout_get_element(&editor->layout, "save_button");
	editor->edit_button = ui_layout_get_element(&editor->layout, "edit_button");

	editor->sector_edit_menu = ui_layout_get_element(&editor->layout, "sector_edit_menu");
	editor->close_sector_edit_button = ui_layout_get_element(&editor->layout, "close_sector_edit_button");
	editor->sector_edit_ok_button = ui_layout_get_element(&editor->layout, "sector_edit_ok_button");
	editor->sector_skybox_dropdown = ui_layout_get_element(&editor->layout, "sector_skybox_dropdown");
	editor->sector_skybox_none = ui_layout_get_element(&editor->layout, "sector_skybox_none");
	editor->sector_skybox_one = ui_layout_get_element(&editor->layout, "sector_skybox_one");
	editor->sector_skybox_two = ui_layout_get_element(&editor->layout, "sector_skybox_two");
	editor->sector_skybox_three = ui_layout_get_element(&editor->layout, "sector_skybox_three");
	editor->floor_texture_button = ui_layout_get_element(&editor->layout, "floor_texture_button");
	editor->ceiling_texture_button = ui_layout_get_element(&editor->layout, "ceiling_texture_button");
	editor->floor_texture_image = ui_layout_get_element(&editor->layout, "floor_texture_image");
	editor->ceiling_texture_image = ui_layout_get_element(&editor->layout, "ceiling_texture_image");
	// Inputs;
	editor->floor_height_input = ui_layout_get_element(&editor->layout, "floor_height_input");
	editor->ceiling_height_input = ui_layout_get_element(&editor->layout, "ceiling_height_input");
	editor->gravity_input = ui_layout_get_element(&editor->layout, "gravity_input");
	editor->lighting_input = ui_layout_get_element(&editor->layout, "lighting_input");
	editor->floor_texture_scale_input = ui_layout_get_element(&editor->layout, "floor_texture_scale_input");
	editor->ceiling_texture_scale_input = ui_layout_get_element(&editor->layout, "ceiling_texture_scale_input");

	// Error
	editor->error_label = ui_layout_get_element(&editor->layout, "error_label");

	// Wall
	editor->menu_wall_edit = ui_layout_get_element(&editor->layout, "menu_wall_edit");
	editor->close_wall_edit_button = ui_layout_get_element(&editor->layout, "close_wall_edit_button");
	editor->solid_checkbox = ui_layout_get_element(&editor->layout, "solidity_checkbox");
	editor->portal_checkbox = ui_layout_get_element(&editor->layout, "portal_checkbox");
	editor->split_wall_button = ui_layout_get_element(&editor->layout, "split_wall_button");
	editor->wall_skybox_dropdown = ui_layout_get_element(&editor->layout, "wall_skybox_dropdown");
	editor->wall_skybox_none = ui_layout_get_element(&editor->layout, "wall_skybox_none");
	editor->wall_skybox_one = ui_layout_get_element(&editor->layout, "wall_skybox_one");
	editor->wall_skybox_two = ui_layout_get_element(&editor->layout, "wall_skybox_two");
	editor->wall_skybox_three = ui_layout_get_element(&editor->layout, "wall_skybox_three");
	editor->wall_texture_button = ui_layout_get_element(&editor->layout, "wall_texture_button");
	editor->wall_texture_image = ui_layout_get_element(&editor->layout, "wall_texture_image");
	editor->portal_texture_button = ui_layout_get_element(&editor->layout, "portal_texture_button");
	editor->portal_texture_image = ui_layout_get_element(&editor->layout, "portal_texture_image");
	editor->floor_wall_angle_input = ui_layout_get_element(&editor->layout, "floor_wall_angle_input");
	editor->ceiling_wall_angle_input = ui_layout_get_element(&editor->layout, "ceiling_wall_angle_input");
	editor->wall_texture_scale_input = ui_layout_get_element(&editor->layout, "wall_texture_scale_input");

	// Sprite
	editor->sprite_edit_menu = ui_layout_get_element(&editor->layout, "sprite_edit_menu");
	editor->sprite_add_button = ui_layout_get_element(&editor->layout, "sprite_add_button");
	editor->sprite_remove_button = ui_layout_get_element(&editor->layout, "sprite_remove_button");
	editor->sprite_scale_input = ui_layout_get_element(&editor->layout, "sprite_scale_input");
	editor->sprite_type_dropdown = ui_layout_get_element(&editor->layout, "sprite_type_dropdown");
	editor->sprite_type_static = ui_layout_get_element(&editor->layout, "sprite_type_static");
	editor->sprite_type_loop = ui_layout_get_element(&editor->layout, "sprite_type_loop");
	editor->sprite_type_action = ui_layout_get_element(&editor->layout, "sprite_type_action");
	editor->sprite_texture_button = ui_layout_get_element(&editor->layout, "sprite_texture_button");
	editor->sprite_texture_image = ui_layout_get_element(&editor->layout, "sprite_texture_image");
	editor->sprite_x_input = ui_layout_get_element(&editor->layout, "sprite_x_input");
	editor->sprite_y_input = ui_layout_get_element(&editor->layout, "sprite_y_input");
	editor->wall_render = ui_layout_get_element(&editor->layout, "wall_render");
	for (int i = 0; i < MAP_TEXTURE_AMOUNT; i++)
	{
		ft_printf("Load Image (%d) : %s\n", i, g_map_textures[i].path);
		editor->wall_textures[i] = load_bxpm_to_surface(g_map_textures[i].path);
	}

	// Texture Menu
	editor->texture_menu = ui_layout_get_element(&editor->layout, "texture_menu");
	ui_element_print(editor->texture_menu);
	editor->texture_menu->show = 0;
	editor->texture_menu_close_button = ui_layout_get_element(&editor->layout, "texture_menu_close_button");
	editor->texture_menu_label = ui_layout_get_element(&editor->layout, "texture_menu_label");

	// Create buttons from all 'wall_textures' and place them on the 'texture_menu';
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

		ui_element_pos_set2(texture_elem->menu,
			align_fill(vec2(editor->texture_menu->pos.w,
					editor->texture_menu->pos.h),
				texture_menu_button_recipe->pos, 5, i));

		texture_elem->image = ft_memalloc(sizeof(t_ui_element));
		ui_menu_new(editor->win_main, texture_elem->image);
		ui_element_set_parent(texture_elem->image, texture_elem->menu, UI_TYPE_ELEMENT);
		ui_element_edit(texture_elem->image, texture_image_recipe);
		ui_element_image_set(texture_elem->image, UI_STATE_DEFAULT, editor->wall_textures[i]);

		texture_elem->button = ft_memalloc(sizeof(t_ui_element));
		ui_button_new(editor->win_main, texture_elem->button);
		ui_element_set_parent(texture_elem->button, texture_elem->menu, UI_TYPE_ELEMENT);
		ui_element_edit(texture_elem->button, texture_button_recipe);

		texture_elem->id = i;

		add_to_list(&editor->texture_elems, texture_elem, sizeof(t_texture_elem));
		add_to_list(&editor->texture_buttons, texture_elem->button, sizeof(t_ui_element));

		ft_printf("[%s] Texture elem #%d made.\n", __FUNCTION__, i);
	}

	// Sprite texture
	editor->sprite_texture_something = ft_memalloc(sizeof(t_texture_something));
	editor->sprite_texture_something->button = editor->sprite_texture_button;
	editor->sprite_texture_something->image = editor->sprite_texture_image;
	add_to_list(&editor->texture_somethings, editor->sprite_texture_something, sizeof(t_texture_something));
	// Wall texture
	editor->wall_texture_something = ft_memalloc(sizeof(t_texture_something));
	editor->wall_texture_something->button = editor->wall_texture_button;
	editor->wall_texture_something->image = editor->wall_texture_image;
	add_to_list(&editor->texture_somethings, editor->wall_texture_something, sizeof(t_texture_something));
	// Portal texture
	editor->portal_texture_something = ft_memalloc(sizeof(t_texture_something));
	editor->portal_texture_something->button = editor->portal_texture_button;
	editor->portal_texture_something->image = editor->portal_texture_image;
	add_to_list(&editor->texture_somethings, editor->portal_texture_something, sizeof(t_texture_something));
	// floor texture
	editor->floor_texture_something = ft_memalloc(sizeof(t_texture_something));
	editor->floor_texture_something->button = editor->floor_texture_button;
	editor->floor_texture_something->image = editor->floor_texture_image;
	add_to_list(&editor->texture_somethings, editor->floor_texture_something, sizeof(t_texture_something));
	// ceiling texture
	editor->ceiling_texture_something = ft_memalloc(sizeof(t_texture_something));
	editor->ceiling_texture_something->button = editor->ceiling_texture_button;
	editor->ceiling_texture_something->image = editor->ceiling_texture_image;
	add_to_list(&editor->texture_somethings, editor->ceiling_texture_something, sizeof(t_texture_something));

	add_to_list(&editor->texture_opening_buttons, editor->sprite_texture_button, UI_TYPE_ELEMENT);
	add_to_list(&editor->texture_opening_buttons, editor->wall_texture_button, UI_TYPE_ELEMENT);
	add_to_list(&editor->texture_opening_buttons, editor->portal_texture_button, UI_TYPE_ELEMENT);
	add_to_list(&editor->texture_opening_buttons, editor->floor_texture_button, UI_TYPE_ELEMENT);
	add_to_list(&editor->texture_opening_buttons, editor->ceiling_texture_button, UI_TYPE_ELEMENT);

	// Entity Edit
	editor->entity_edit_menu = ui_layout_get_element(&editor->layout, "entity_edit_menu");
	editor->close_entity_edit_button = ui_layout_get_element(&editor->layout, "close_entity_edit_button");
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
	editor->entity_dropdown = ui_layout_get_element(&editor->layout, "entity_dropdown");
	editor->entity_image = ui_layout_get_element(&editor->layout, "entity_image");
	editor->entity_z_input = ui_layout_get_element(&editor->layout, "entity_z_input");
	editor->entity_yaw_input = ui_layout_get_element(&editor->layout, "entity_yaw_input");
	editor->entity_yaw_slider = ui_layout_get_element(&editor->layout, "entity_yaw_slider");

	editor->sector_info_label = ui_layout_get_element(&editor->layout, "selected_sector_info");
	ui_label_get_label(editor->sector_info_label)->max_w = editor->sector_info_label->pos.w;
	editor->mouse_info_label = ui_layout_get_element(&editor->layout, "mouse_hover_info");
	editor->sub_info_label = ui_layout_get_element(&editor->layout, "selected_sub_info");
	ui_label_get_label(editor->sub_info_label)->max_w = editor->sub_info_label->pos.w;
	editor->sprite_info_label = ui_layout_get_element(&editor->layout, "selected_sprite_info");
	ui_label_get_label(editor->sprite_info_label)->max_w = editor->sprite_info_label->pos.w;
	editor->misc_info_label = ui_layout_get_element(&editor->layout, "misc_info");
	ui_label_get_label(editor->misc_info_label)->max_w = editor->misc_info_label->pos.w;

	// Global Info (used for telling user you have saved map for example)
	editor->info_label = ui_layout_get_element(&editor->layout, "info_label");

	// Sector Hover Info
	editor->sector_hover_info_menu = ui_layout_get_element(&editor->layout, "sector_hover_info_menu");
	editor->sector_hover_info_label = ui_layout_get_element(&editor->layout, "sector_hover_info_label");

	// Event edit
	editor->event_scrollbar = ui_layout_get_element(&editor->layout, "event_scrollbar");
	editor->event_edit_menu = ui_layout_get_element(&editor->layout, "event_edit_menu");
	editor->add_event_button = ui_layout_get_element(&editor->layout, "add_event_button");
	editor->remove_event_button = ui_layout_get_element(&editor->layout, "remove_event_button");
	editor->event_type_dropdown = ui_layout_get_element(&editor->layout, "event_type_dropdown");
	editor->event_action_dropdown = ui_layout_get_element(&editor->layout, "event_action_dropdown");
	editor->event_id_dropdown = ui_layout_get_element(&editor->layout, "event_id_dropdown");
	editor->event_id_menu = ui_dropdown_get_menu_element(editor->event_id_dropdown);
	editor->event_sector_input = ui_layout_get_element(&editor->layout, "event_sector_input");
	editor->event_min_input = ui_layout_get_element(&editor->layout, "event_min_input");
	editor->event_max_input = ui_layout_get_element(&editor->layout, "event_max_input");
	editor->event_speed_input = ui_layout_get_element(&editor->layout, "event_speed_input");
	editor->event_menu = ui_layout_get_element(&editor->layout, "event_menu");

	// types
	editor->event_type_floor = ui_layout_get_element(&editor->layout, "event_type_floor");
	editor->event_type_ceiling = ui_layout_get_element(&editor->layout, "event_type_ceiling");
	editor->event_type_light = ui_layout_get_element(&editor->layout, "event_type_light");
	editor->event_type_store = ui_layout_get_element(&editor->layout, "event_type_store");
	editor->event_type_hazard = ui_layout_get_element(&editor->layout, "event_type_hazard");
	editor->event_type_audio = ui_layout_get_element(&editor->layout, "event_type_audio");
	editor->event_type_spawn = ui_layout_get_element(&editor->layout, "event_type_spawn");
	// actions
	editor->event_action_click_button = ui_layout_get_element(&editor->layout, "event_action_click");
	editor->event_action_shoot_button = ui_layout_get_element(&editor->layout, "event_action_shoot");
	editor->event_action_sector_button = ui_layout_get_element(&editor->layout, "event_action_sector");
	editor->event_action_null_button = ui_layout_get_element(&editor->layout, "event_action_null");

	// Spawn Edit
	editor->spawn_edit_menu = ui_layout_get_element(&editor->layout, "spawn_edit_menu");
	editor->spawn_yaw_input = ui_layout_get_element(&editor->layout, "spawn_yaw_input");

	// Save Window
	editor->win_save = ui_layout_get_window(&editor->layout, "win_save");
	editor->endless_checkbox = ui_layout_get_element(&editor->layout, "endless_checkbox");
	editor->story_checkbox = ui_layout_get_element(&editor->layout, "story_checkbox");
	editor->name_input = ui_layout_get_element(&editor->layout, "name_input");
	editor->confirm_save_button = ui_layout_get_element(&editor->layout, "confirm_save_button");

	// Edit Window
	editor->win_edit = ui_layout_get_window(&editor->layout, "win_edit");
	editor->map_scale_input = ui_layout_get_element(&editor->layout, "map_scale_input");
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
			if (e.key.keysym.scancode == SDL_SCANCODE_P)
				print_entities(editor.entities);
		}
		user_render(&editor);
		ui_layout_render(&editor.layout);
	}
}
