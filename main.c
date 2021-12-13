/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:09 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 13:50:39 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	t_list			*next_butt;

	i = -1;
	prev_butt = parent->children;
	while (texts[++i])
	{
		if (prev_butt && prev_butt->content)
		{
			ui_button_set_text(prev_butt->content, texts[i]);
			prev_butt = prev_butt->next;
		}
		else
			new_id_button(parent, texts[i], recipe);
	}
	while (prev_butt)
	{
		next_butt= prev_butt->next;
		ui_element_free(prev_butt->content, prev_butt->content_size);
		if (!prev_butt)
			break ;
		prev_butt = next_butt;
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

void	setup_ui_values(t_editor *editor)
{
	ui_input_set_text(editor->name_input, editor->map_name);
	realign_event_buttons(editor);
}

void	editor_free(t_editor *editor)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ui_layout_free(&editor->layout);
	ft_printf("Layout freed.");
	ft_lstdel(&editor->texture_somethings, &dummy_free_er);
	ft_printf("Texture Something freed.");
	ft_lstdel(&editor->texture_opening_buttons, &dummy_free_er);
	ft_printf("Texture Opening freed.");
	ft_lstdel(&editor->texture_elems, &texture_elem_free);
	ft_printf("Texture Elems freed.");
	ft_lstdel(&editor->texture_buttons, &dummy_free_er);
	ft_printf("Texture Buttons freed.");
	TTF_CloseFont(editor->font);
	ft_printf("Font freed.");
	SDL_DestroyTexture(editor->drawing_texture);
	ft_printf("Drawing Texture freed.");
	SDL_FreeSurface(editor->drawing_surface);
	ft_printf("Drawing Surface freed.");
	SDL_FreeSurface(editor->grid_surface);
	ft_printf("Grid Surface freed.");
	surface_array_free(editor->wall_textures, MAP_TEXTURE_AMOUNT);
	ft_printf("Wall textures freed.");
	texture_array_free(editor->entity_textures, ENTITY_AMOUNT + 1);
	ft_printf("Entity textures freed.");
	surface_array_free(editor->entity_texture_surfaces, ENTITY_AMOUNT + 1);
	ft_printf("Entity texture surfaces freed.");
	ft_lstdel(&editor->sectors, &sector_free);
	ft_printf("Sectors freed.");
	ft_lstdel(&editor->walls, &dummy_free_er);
	ft_lstdel(&editor->points, &point_free);
	ft_lstdel(&editor->sprites, &dummy_free_er);
	ft_lstdel(&editor->entities, &entity_free);
	ft_printf("Entity freed.");
	ft_lstdel(&editor->events, &event_free);
	ft_printf("Event freed.");
	ft_lstdel(&editor->event_elements, &event_elem_free);
	ft_printf("Event elem freed.");
	ft_lstdel(&editor->event_element_buttons, &dummy_free_er);
	ft_printf("Event elem buttons freed.");
	free(editor->map_name);
	free(editor->map_full_path);
	ft_printf("Map name freed.");
	ui_sdl_free();
}

int	realmain(int ac, char **av)
{
	t_editor	editor;
	t_fps		fps;

	ui_sdl_init();
	editor_init(&editor);
	draw_init(&editor);
	if (args_parser(&editor, ac, av))
		get_map(&editor, editor.map_full_path);
	else
		ft_printf("[%s] No map given.\n", __FUNCTION__);
	setup_ui_values(&editor);
	memset(&fps, 0, sizeof(t_fps));
	while (!editor.win_main->wants_to_close)
	{
		fps_func(&fps);
		update_title_fps(editor.win_main->win, &fps);
		while (SDL_PollEvent(&editor.e))
		{
			if (editor.e.key.keysym.scancode == SDL_SCANCODE_E)
				editor.win_main->wants_to_close = 1;
			ui_layout_event(&editor.layout, editor.e);
			user_events(&editor, editor.e);
		}
		user_render(&editor);
		ui_layout_render(&editor.layout);
	}
		/*
		*/
	editor_free(&editor);
	return (1);
}

int	main(int ac, char **av)
{
	realmain(ac, av);
	system("leaks doom_editor");
	return (1);
}
