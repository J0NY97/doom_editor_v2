/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_menu_events.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:44:29 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:44:29 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_texture_elem	*get_active_texture_elem(t_editor *editor)
{
	t_texture_elem	*new_texture_elem;
	t_list			*curr;

	if (ui_list_radio_event(editor->texture_buttons,
			&editor->active_texture_button))
	{
		curr = editor->texture_elems;
		while (curr)
		{
			new_texture_elem = curr->content;
			if (&new_texture_elem->button == editor->active_texture_button)
			{
				editor->active_texture_button_id = new_texture_elem->id;
				return (new_texture_elem);
			}
			curr = curr->next;
		}
	}
	return (NULL);
}

void	texture_menu_select_events(t_editor *editor)
{
	t_list				*curr;
	t_texture_elem		*selected_texture_elem;
	t_texture_comb		*t_something;

	selected_texture_elem = get_active_texture_elem(editor);
	if (selected_texture_elem
		&& editor->active_texture_button_id > -1)
	{
		t_something = NULL;
		curr = editor->texture_somethings;
		while (curr)
		{
			t_something = curr->content;
			if (t_something->button == editor->active_texture_opening_button)
			{
				t_something->id = selected_texture_elem->id;
				ui_element_image_set(t_something->image, UI_STATE_DEFAULT,
					editor->wall_textures[selected_texture_elem->id]);
				break ;
			}
			curr = curr->next;
		}
	}
}

void	texture_menu_events(t_editor *editor)
{
	if (ui_list_radio_event(editor->texture_opening_buttons,
			&editor->active_texture_opening_button))
	{
		ui_label_set_text(editor->texture_menu_label,
			ui_button_get_text(editor->active_texture_opening_button));
		editor->active_texture_button = NULL;
		editor->active_texture_button_id = -1;
		editor->texture_menu->show = 1;
	}
	if (!editor->texture_menu->show
		|| !editor->active_texture_opening_button
		|| editor->texture_menu_close_button->state == UI_STATE_CLICK)
	{
		editor->texture_menu->show = 0;
		if (editor->active_texture_opening_button)
			editor->active_texture_opening_button->state = UI_STATE_DEFAULT;
		editor->active_texture_opening_button = NULL;
		editor->active_texture_button_id = -1;
		return ;
	}
	texture_menu_select_events(editor);
}
