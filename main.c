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
