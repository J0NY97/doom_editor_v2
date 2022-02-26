/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:09 by nneronin          #+#    #+#             */
/*   Updated: 2022/02/26 08:30:36 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	send_info_message(t_editor *editor, char *text)
{
	editor->info_label_start_timer = SDL_GetTicks();
	editor->info_label_timer = 0;
	ui_label_color_set(editor->info_label, 0xffffffff);
	ui_label_set_text(editor->info_label, text);
	editor->info_label->show = 1;
}

void	setup_ui_values(t_editor *editor)
{
	ui_input_set_text(editor->name_input, editor->map_name);
	realign_event_buttons(editor);
}

void	editor_free(t_editor *editor)
{
	LG_DEBUG("[%s] Freeing\n", __FUNCTION__);
	ui_layout_free(&editor->layout);
	ft_lstdel(&editor->texture_somethings, &dummy_free_er);
	ft_lstdel(&editor->texture_opening_buttons, &dummy_free_er);
	ft_lstdel(&editor->texture_elems, &texture_elem_free);
	ft_lstdel(&editor->texture_buttons, &dummy_free_er);
	SDL_DestroyTexture(editor->drawing_texture);
	SDL_FreeSurface(editor->drawing_surface);
	SDL_FreeSurface(editor->grid_surface);
	surface_array_free(editor->wall_textures, MAP_TEXTURE_AMOUNT);
	texture_array_free(editor->entity_textures, ENTITY_AMOUNT + 1);
	surface_array_free(editor->entity_texture_surfaces, ENTITY_AMOUNT + 1);
	ft_lstdel(&editor->sectors, &sector_free);
	ft_lstdel(&editor->walls, &dummy_free_er);
	ft_lstdel(&editor->points, &point_free);
	ft_lstdel(&editor->sprites, &dummy_free_er);
	ft_lstdel(&editor->entities, &entity_free);
	ft_lstdel(&editor->events, &event_free);
	ft_lstdel(&editor->event_elements, &event_elem_free);
	ft_lstdel(&editor->event_element_buttons, &dummy_free_er);
	free(editor->map_name);
	free(editor->map_full_path);
	ui_sdl_free();
}

void	map_init(t_editor *editor, int ac, char **av)
{
	if (args_parser(editor, ac, av))
		get_map(editor, editor->map_full_path);
	else
		LG_INFO("[%s] No map given.", __FUNCTION__);
}

int	main(int ac, char **av)
{
	t_editor	editor;

	ui_sdl_init();
	editor_init(&editor);
	draw_init(&editor);
	map_init(&editor, ac, av);
	setup_ui_values(&editor);
	while (!editor.win_main->wants_to_close)
	{
		fps_func(&editor.fps);
		update_title_fps(editor.win_main->win, &editor.fps);
		while (SDL_PollEvent(&editor.e))
		{
			ui_layout_event(&editor.layout, editor.e);
			user_events(&editor, editor.e);
			if (editor.e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				editor.win_main->wants_to_close = 1;
		}
		user_render(&editor);
		ui_layout_render(&editor.layout);
	}
	editor_free(&editor);
	launcher(&editor);
	return (1);
}

/*
int	main(int ac, char **av)
{
	realmain(ac, av);
	system("leaks doom_editor");
	return (1);
}
*/
