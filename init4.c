/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 14:31:39 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/20 14:41:34 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	texture_elem_init(t_editor *editor)
{
	t_texture_elem	*tex_elem;
	int				i;

	i = -1;
	while (++i < MAP_TEXTURE_AMOUNT)
	{
		tex_elem = ft_memalloc(sizeof(t_texture_elem));
		set_elem_parent_and_recipe(&tex_elem->menu, UI_TYPE_MENU,
			editor->texture_menu, "texture_button_menu");
		ui_element_pos_set2(&tex_elem->menu, align_fill(
				vec2(editor->texture_menu->pos.w, editor->texture_menu->pos.h),
				tex_elem->menu.pos, 5, i));
		set_elem_parent_and_recipe(&tex_elem->image, UI_TYPE_MENU,
			&tex_elem->menu, "texture_image");
		ui_element_image_set(&tex_elem->image, UI_STATE_DEFAULT,
			editor->wall_textures[i]);
		set_elem_parent_and_recipe(&tex_elem->button, UI_TYPE_BUTTON,
			&tex_elem->menu, "texture_button");
		tex_elem->id = i;
		add_to_list(&editor->texture_elems, tex_elem, sizeof(t_texture_elem));
		add_to_list(&editor->texture_buttons, &tex_elem->button,
			sizeof(t_ui_element));
	}
}

void	new_texture_something(
		t_editor *editor, t_texture_comb *something,
		t_ui_element *button, t_ui_element *image)
{
	something->button = button;
	something->image = image;
	add_to_list(&editor->texture_somethings,
		something, sizeof(t_texture_comb));
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
		= ui_surface_image_new(ICON_PATH"damage.png");
	editor->entity_textures[0] = ui_texture_create_from_path(
			editor->win_main->renderer, ICON_PATH"damage.png");
	while (++i < ENTITY_AMOUNT)
	{
		ft_printf("Loading Entity Image : %s\n", g_entity_data[i].path);
		surface = load_bxpm_to_surface(g_entity_data[i].path);
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
