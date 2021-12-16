/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:05 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 11:45:22 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

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
	memset(&editor->fps, 0, sizeof(t_fps));
	ui_layout_load(&editor->layout, EDITOR_PATH"layout.ui");
	if (!editor->layout.style_file_content
		|| !editor->layout.layout_file_content)
		exit(0);
	editor->map_type = MAP_TYPE_ENDLESS;
	editor->map_name = ft_strdup("");
	editor->win_main = ui_layout_get_window(&editor->layout, "win_main");
	type_dropdown_init(editor);
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
	editor->offset = vec2(0, 0);
	editor->grid_surface = ui_surface_new(
			editor->drawing_surface->w, editor->drawing_surface->h);
	editor->update_grid = 1;
}
