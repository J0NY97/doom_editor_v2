/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:05 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/18 12:35:05 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	load_map_textures(t_editor *editor)
{
	int	i;

	i = -1;
	while (++i < MAP_TEXTURE_AMOUNT)
	{
		LG_DEBUG("Load Image (%d) : %s", i, g_map_textures[i].path);
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

void	misc_init(t_editor *editor)
{
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
}

void	editor_init(t_editor *editor)
{
	lg_setLevel(1, 0);
	lg_openFile("log.log", "a+");
	memset(editor, 0, sizeof(t_editor));
	memset(&editor->fps, 0, sizeof(t_fps));
	ui_layout_load(&editor->layout, EDITOR_PATH, "layout.ui");
	if (!editor->layout.style_file_content
		|| !editor->layout.layout_file_content)
		LG_ERROR("[%s] Couldn\'t load layout file.", __FUNCTION__);
	editor->map_type = MAP_TYPE_ENDLESS;
	editor->map_name = ft_strdup("");
	editor->win_main = ui_layout_get_window(&editor->layout, "win_main");
	ui_window_set_icon(editor->win_main, EDITOR_PATH"DNDe.bmp");
	type_dropdown_init(editor);
	selection_menu_init(editor);
	sector_edit_init(editor);
	editor->error_label = ui_layout_get_element(&editor->layout, "error_label");
	misc_init(editor);
	LG_DEBUG("[%s] %s\n", __FUNCTION__, SDL_GetError());
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
