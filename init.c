#include "editor.h"

void	selection_menu_init(t_editor *editor)
{
	editor->menu_toolbox_top
		= ui_layout_get_element(&editor->layout, "menu_toolbox_top");
	editor->menu_selection
		= ui_layout_get_element(&editor->layout, "menu_select_buttons");
	editor->selection_dropdown_menu
		= ui_layout_get_element(&editor->layout, "type_drop_menu");
	editor->draw_button
		= ui_layout_get_element(&editor->layout, "draw_button");
	editor->select_button
		= ui_layout_get_element(&editor->layout, "select_button");
	editor->remove_button
		= ui_layout_get_element(&editor->layout, "remove_button");
	editor->sector_button
		= ui_layout_get_element(&editor->layout, "sector_button");
	editor->entity_button
		= ui_layout_get_element(&editor->layout, "entity_button");
	editor->spawn_button
		= ui_layout_get_element(&editor->layout, "spawn_button");
	editor->event_button
		= ui_layout_get_element(&editor->layout, "event_button");
	editor->save_button
		= ui_layout_get_element(&editor->layout, "save_button");
	editor->edit_button
		= ui_layout_get_element(&editor->layout, "edit_button");
}

void	sector_skybox_init(t_editor *editor)
{
	editor->sector_skybox_dropdown
		= ui_layout_get_element(&editor->layout, "sector_skybox_dropdown");
	editor->sector_skybox_none
		= ui_layout_get_element(&editor->layout, "sector_skybox_none");
	editor->sector_skybox_one
		= ui_layout_get_element(&editor->layout, "sector_skybox_one");
	editor->sector_skybox_two
		= ui_layout_get_element(&editor->layout, "sector_skybox_two");
	editor->sector_skybox_three
		= ui_layout_get_element(&editor->layout, "sector_skybox_three");
}

void	sector_edit_init2(t_editor *editor)
{
	editor->point_button
		= ui_layout_get_element(&editor->layout, "point_button");
	editor->wall_button
		= ui_layout_get_element(&editor->layout, "wall_button");
	editor->close_sector_edit_button
		= ui_layout_get_element(&editor->layout, "close_sector_edit_button");
	editor->sector_edit_ok_button
		= ui_layout_get_element(&editor->layout, "sector_edit_ok_button");
}

void	sector_edit_init(t_editor *editor)
{
	editor->sector_edit_menu
		= ui_layout_get_element(&editor->layout, "sector_edit_menu");
	editor->floor_texture_button
		= ui_layout_get_element(&editor->layout, "floor_texture_button");
	editor->ceiling_texture_button
		= ui_layout_get_element(&editor->layout, "ceiling_texture_button");
	editor->floor_texture_image
		= ui_layout_get_element(&editor->layout, "floor_texture_image");
	editor->ceiling_texture_image
		= ui_layout_get_element(&editor->layout, "ceiling_texture_image");
	editor->floor_height_input
		= ui_layout_get_element(&editor->layout, "floor_height_input");
	editor->ceiling_height_input
		= ui_layout_get_element(&editor->layout, "ceiling_height_input");
	editor->gravity_input
		= ui_layout_get_element(&editor->layout, "gravity_input");
	editor->lighting_input
		= ui_layout_get_element(&editor->layout, "lighting_input");
	editor->floor_texture_scale_input = ui_layout_get_element(
			&editor->layout, "floor_texture_scale_input");
	editor->ceiling_texture_scale_input = ui_layout_get_element(
			&editor->layout, "ceiling_texture_scale_input");
	sector_edit_init2(editor);
	sector_skybox_init(editor);
}

void	wall_skybox_init(t_editor *editor)
{
	editor->wall_skybox_dropdown
		= ui_layout_get_element(&editor->layout, "wall_skybox_dropdown");
	editor->wall_skybox_none
		= ui_layout_get_element(&editor->layout, "wall_skybox_none");
	editor->wall_skybox_one
		= ui_layout_get_element(&editor->layout, "wall_skybox_one");
	editor->wall_skybox_two
		= ui_layout_get_element(&editor->layout, "wall_skybox_two");
	editor->wall_skybox_three
		= ui_layout_get_element(&editor->layout, "wall_skybox_three");
}

void	wall_edit_init(t_editor *editor)
{
	editor->menu_wall_edit
		= ui_layout_get_element(&editor->layout, "menu_wall_edit");
	editor->close_wall_edit_button
		= ui_layout_get_element(&editor->layout, "close_wall_edit_button");
	editor->solid_checkbox
		= ui_layout_get_element(&editor->layout, "solidity_checkbox");
	editor->portal_checkbox
		= ui_layout_get_element(&editor->layout, "portal_checkbox");
	editor->split_wall_button
		= ui_layout_get_element(&editor->layout, "split_wall_button");
	editor->wall_texture_button
		= ui_layout_get_element(&editor->layout, "wall_texture_button");
	editor->wall_texture_image
		= ui_layout_get_element(&editor->layout, "wall_texture_image");
	editor->portal_texture_button
		= ui_layout_get_element(&editor->layout, "portal_texture_button");
	editor->portal_texture_image
		= ui_layout_get_element(&editor->layout, "portal_texture_image");
	editor->floor_wall_angle_input
		= ui_layout_get_element(&editor->layout, "floor_wall_angle_input");
	editor->ceiling_wall_angle_input = ui_layout_get_element(
			&editor->layout, "ceiling_wall_angle_input");
	editor->wall_texture_scale_input = ui_layout_get_element(
			&editor->layout, "wall_texture_scale_input");
	wall_skybox_init(editor);
}

void	wall_render_init(t_editor *editor)
{
	editor->wall_render
		= ui_layout_get_element(&editor->layout, "wall_render");
}

void	sprite_edit_init(t_editor *editor)
{
	editor->sprite_edit_menu
		= ui_layout_get_element(&editor->layout, "sprite_edit_menu");
	editor->sprite_add_button
		= ui_layout_get_element(&editor->layout, "sprite_add_button");
	editor->sprite_remove_button
		= ui_layout_get_element(&editor->layout, "sprite_remove_button");
	editor->sprite_scale_input
		= ui_layout_get_element(&editor->layout, "sprite_scale_input");
	editor->sprite_type_dropdown
		= ui_layout_get_element(&editor->layout, "sprite_type_dropdown");
	editor->sprite_type_static
		= ui_layout_get_element(&editor->layout, "sprite_type_static");
	editor->sprite_type_loop
		= ui_layout_get_element(&editor->layout, "sprite_type_loop");
	editor->sprite_type_action
		= ui_layout_get_element(&editor->layout, "sprite_type_action");
	editor->sprite_texture_button
		= ui_layout_get_element(&editor->layout, "sprite_texture_button");
	editor->sprite_texture_image
		= ui_layout_get_element(&editor->layout, "sprite_texture_image");
	editor->sprite_x_input
		= ui_layout_get_element(&editor->layout, "sprite_x_input");
	editor->sprite_y_input
		= ui_layout_get_element(&editor->layout, "sprite_y_input");
	wall_render_init(editor);
}

void	texture_elem_init(t_editor *editor)
{
	t_texture_elem	*texture_elem;
	int				i;

	i = -1;
	while (++i < MAP_TEXTURE_AMOUNT)
	{
		texture_elem = ft_memalloc(sizeof(t_texture_elem));
		set_elem_parent_and_recipe(&texture_elem->menu, UI_TYPE_MENU,
			editor->texture_menu, "texture_button_menu");
		ui_element_pos_set2(&texture_elem->menu, align_fill(
				vec2(editor->texture_menu->pos.w, editor->texture_menu->pos.h),
				texture_elem->menu.pos, 5, i));
		set_elem_parent_and_recipe(&texture_elem->image, UI_TYPE_MENU,
			&texture_elem->menu, "texture_image");
		ui_element_image_set(&texture_elem->image, UI_STATE_DEFAULT,
			editor->wall_textures[i]);
		texture_elem->button = ft_memalloc(sizeof(t_ui_element));
		set_elem_parent_and_recipe(texture_elem->button, UI_TYPE_BUTTON,
			&texture_elem->menu, "texture_button");
		texture_elem->id = i;
		add_to_list(&editor->texture_elems,
			texture_elem, sizeof(t_texture_elem));
		add_to_list(&editor->texture_buttons,
			texture_elem->button, sizeof(t_ui_element));
	}
}

void	new_texture_something(
		t_editor *editor, t_texture_something *something,
		t_ui_element *button, t_ui_element *image)
{
	something->button = button;
	something->image = image;
	add_to_list(&editor->texture_somethings,
		something, sizeof(t_texture_something));
	add_to_list(&editor->texture_opening_buttons, button, UI_TYPE_ELEMENT);
}

void	texture_menu_init(t_editor *editor)
{
	editor->texture_menu
		= ui_layout_get_element(&editor->layout, "texture_menu");
	editor->texture_menu_close_button = ui_layout_get_element(
			&editor->layout, "texture_menu_close_button");
	editor->texture_menu_label
		= ui_layout_get_element(&editor->layout, "texture_menu_label");
	texture_elem_init(editor);
	new_texture_something(editor, &editor->sprite_texture_something,
		editor->sprite_texture_button, editor->sprite_texture_image);
	new_texture_something(editor, &editor->wall_texture_something,
		editor->wall_texture_button, editor->wall_texture_image);
	new_texture_something(editor, &editor->portal_texture_something,
		editor->portal_texture_button, editor->portal_texture_image);
	new_texture_something(editor, &editor->floor_texture_something,
		editor->floor_texture_button, editor->floor_texture_image);
	new_texture_something(editor, &editor->ceiling_texture_something,
		editor->ceiling_texture_button, editor->ceiling_texture_image);
}

void	load_entity_textures(t_editor *editor)
{
	int			i;
	t_vec4i		pos;
	SDL_Surface	*surface;

	i = -1;
	editor->entity_texture_surfaces[0]
		= ui_surface_image_new("ui_images/damage.png");
	editor->entity_textures[0]
		= ui_texture_create_from_path(
			editor->win_main->renderer, "ui_images/damage.png");
	while (++i < ENTITY_AMOUNT)
	{
		ft_printf("Load Entity Image : %s ", g_entity_data[i].path);
		surface = ui_surface_image_new(g_entity_data[i].path);
		editor->entity_texture_surfaces[i + 1] = ui_surface_new(10, 10);
		pos.x = g_entity_data[i].tc[0];
		pos.y = g_entity_data[i].tc[1];
		pos.w = g_entity_data[i].tc[2];
		pos.h = g_entity_data[i].tc[3];
		SDL_BlitScaled(surface, &(SDL_Rect){pos.x, pos.y, pos.w, pos.h},
			editor->entity_texture_surfaces[i + 1], NULL);
		editor->entity_textures[i + 1]
			= SDL_CreateTextureFromSurface(editor->win_main->renderer,
				editor->entity_texture_surfaces[i + 1]);
		SDL_FreeSurface(surface);
		ft_printf("- success.\n");
	}
}

void	entity_menu_init(t_editor *editor)
{
	editor->entity_edit_menu
		= ui_layout_get_element(&editor->layout, "entity_edit_menu");
	editor->close_entity_edit_button = ui_layout_get_element(
			&editor->layout, "close_entity_edit_button");
	load_entity_textures(editor);
	editor->entity_dropdown
		= ui_layout_get_element(&editor->layout, "entity_dropdown");
	editor->entity_image
		= ui_layout_get_element(&editor->layout, "entity_image");
	editor->entity_z_input
		= ui_layout_get_element(&editor->layout, "entity_z_input");
	editor->entity_yaw_input
		= ui_layout_get_element(&editor->layout, "entity_yaw_input");
	editor->entity_yaw_slider
		= ui_layout_get_element(&editor->layout, "entity_yaw_slider");
}

void	info_init(t_editor *editor)
{
	editor->sector_info_label
		= ui_layout_get_element(&editor->layout, "selected_sector_info");
	editor->mouse_info_label
		= ui_layout_get_element(&editor->layout, "mouse_hover_info");
	editor->sub_info_label
		= ui_layout_get_element(&editor->layout, "selected_sub_info");
	editor->sprite_info_label
		= ui_layout_get_element(&editor->layout, "selected_sprite_info");
	editor->misc_info_label
		= ui_layout_get_element(&editor->layout, "misc_info");
	editor->info_label
		= ui_layout_get_element(&editor->layout, "info_label");
	editor->sector_hover_info_menu
		= ui_layout_get_element(&editor->layout, "sector_hover_info_menu");
	editor->sector_hover_info_label
		= ui_layout_get_element(&editor->layout, "sector_hover_info_label");
}

void	event_type_init(t_editor *editor)
{
	editor->event_type_dropdown
		= ui_layout_get_element(&editor->layout, "event_type_dropdown");
	editor->event_type_floor
		= ui_layout_get_element(&editor->layout, "event_type_floor");
	editor->event_type_ceiling
		= ui_layout_get_element(&editor->layout, "event_type_ceiling");
	editor->event_type_light
		= ui_layout_get_element(&editor->layout, "event_type_light");
	editor->event_type_store
		= ui_layout_get_element(&editor->layout, "event_type_store");
	editor->event_type_hazard
		= ui_layout_get_element(&editor->layout, "event_type_hazard");
	editor->event_type_audio
		= ui_layout_get_element(&editor->layout, "event_type_audio");
	editor->event_type_spawn
		= ui_layout_get_element(&editor->layout, "event_type_spawn");
}

void	event_action_init(t_editor *editor)
{
	editor->event_action_dropdown
		= ui_layout_get_element(&editor->layout, "event_action_dropdown");
	editor->event_action_click_button
		= ui_layout_get_element(&editor->layout, "event_action_click");
	editor->event_action_shoot_button
		= ui_layout_get_element(&editor->layout, "event_action_shoot");
	editor->event_action_sector_button
		= ui_layout_get_element(&editor->layout, "event_action_sector");
	editor->event_action_null_button
		= ui_layout_get_element(&editor->layout, "event_action_null");
}

void	event_menu_init(t_editor *editor)
{
	editor->event_scrollbar
		= ui_layout_get_element(&editor->layout, "event_scrollbar");
	editor->event_edit_menu
		= ui_layout_get_element(&editor->layout, "event_edit_menu");
	editor->add_event_button
		= ui_layout_get_element(&editor->layout, "add_event_button");
	editor->remove_event_button
		= ui_layout_get_element(&editor->layout, "remove_event_button");
	editor->event_id_dropdown
		= ui_layout_get_element(&editor->layout, "event_id_dropdown");
	editor->event_id_menu
		= ui_dropdown_get_menu_element(editor->event_id_dropdown);
	editor->event_sector_input
		= ui_layout_get_element(&editor->layout, "event_sector_input");
	editor->event_min_input
		= ui_layout_get_element(&editor->layout, "event_min_input");
	editor->event_max_input
		= ui_layout_get_element(&editor->layout, "event_max_input");
	editor->event_speed_input
		= ui_layout_get_element(&editor->layout, "event_speed_input");
	editor->event_menu
		= ui_layout_get_element(&editor->layout, "event_menu");
	event_type_init(editor);
	event_action_init(editor);
}

void	save_window_init(t_editor *editor)
{
	editor->win_save
		= ui_layout_get_window(&editor->layout, "win_save");
	editor->endless_checkbox
		= ui_layout_get_element(&editor->layout, "endless_checkbox");
	editor->story_checkbox
		= ui_layout_get_element(&editor->layout, "story_checkbox");
	editor->name_input
		= ui_layout_get_element(&editor->layout, "name_input");
	editor->confirm_save_button
		= ui_layout_get_element(&editor->layout, "confirm_save_button");
}

void	edit_window_init(t_editor *editor)
{
	editor->win_edit
		= ui_layout_get_window(&editor->layout, "win_edit");
	editor->map_scale_input
		= ui_layout_get_element(&editor->layout, "map_scale_input");
	editor->map_scale = 1.0f;
}

void	load_map_textures(t_editor *editor)
{
	int	i;

	i = -1;
	while (++i < MAP_TEXTURE_AMOUNT)
	{
		ft_printf("Load Image (%d) : %s\n", i, g_map_textures[i].path);
		editor->wall_textures[i] = load_bxpm_to_surface(g_map_textures[i].path);
	}
}

void	spawn_init(t_editor *editor)
{
	editor->spawn_edit_menu
		= ui_layout_get_element(&editor->layout, "spawn_edit_menu");
	editor->spawn_yaw_input
		= ui_layout_get_element(&editor->layout, "spawn_yaw_input");
}

void	editor_init(t_editor *editor)
{
	memset(editor, 0, sizeof(t_editor));
	ui_layout_load(&editor->layout, EDITOR_PATH"layout.ui");
	editor->font = TTF_OpenFont(UI_PATH"fonts/DroidSans.ttf", 20);
	editor->map_type = MAP_TYPE_ENDLESS;
	editor->map_name = ft_strdup("map_name.dnd");
	editor->win_main = ui_layout_get_window(&editor->layout, "win_main");
	selection_menu_init(editor);
	sector_edit_init(editor);
	editor->error_label = ui_layout_get_element(&editor->layout, "error_label");
	wall_edit_init(editor);
	sprite_edit_init(editor);
	load_map_textures(editor);
	texture_menu_init(editor);
	entity_menu_init(editor);
	info_init(editor);
	event_menu_init(editor);
	spawn_init(editor);
	save_window_init(editor);
	edit_window_init(editor);
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
