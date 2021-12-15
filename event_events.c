/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:46:20 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:46:20 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	event_removing_events(t_editor *editor)
{
	if (editor->remove_event_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_event && editor->selected_event_elem)
		{
			remove_event(editor, editor->selected_event);
			editor->selected_event_elem = NULL;
			editor->active_event_elem = NULL;
			editor->selected_event = NULL;
		}
	}
}

/*
 * if we have selected event elem,
 * 		we update it,
 * 	otherwise we make new event elem;
*/
void	event_adding_events(t_editor *editor)
{
	if (editor->add_event_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_event_elem == NULL)
		{
			editor->selected_event = add_event(editor);
			editor->selected_event_elem = editor->selected_event->elem;
			editor->active_event_elem = &editor->selected_event->elem->button;
		}
		update_event(editor, editor->selected_event_elem->event);
		update_event_elem(editor->selected_event_elem);
		editor->selected_event_elem = NULL;
		editor->active_event_elem = NULL;
		editor->selected_event = NULL;
	}
}

/*
 * Update ID Dropdown;
 *	when id dropdown opens we add buttons for each sprite/sector id,
 *	depending on which action type is selected;
*/
void	event_id_dropdown_update(t_editor *editor)
{
	char		**texts;
	t_ui_recipe	*recipe;

	if (editor->event_id_dropdown->show
		&& ui_dropdown_open(editor->event_id_dropdown))
	{
		texts = NULL;
		if (ui_dropdown_active(editor->event_action_dropdown)
			== editor->event_action_sector_button)
			texts = gen_sector_id_texts(editor->sectors);
		else
			texts = gen_sprite_id_texts(editor->sprites);
		recipe = ui_list_get_recipe_by_id(
				editor->layout.recipes, "event_id_button");
		ft_printf("[%s] Creating new id buttons.\n", __FUNCTION__);
		create_buttons_to_list_from_texts_remove_extra(
			ui_dropdown_get_menu_element(editor->event_id_dropdown),
			texts, recipe);
		ft_printf("[%s] Done Creating new id buttons.\n", __FUNCTION__);
		ft_arraydel(texts);
	}
}

void	event_select(t_editor *editor)
{
	t_list	*curr;

	if (ui_list_radio_event(editor->event_element_buttons,
			&editor->active_event_elem))
	{
		curr = editor->event_elements;
		while (curr)
		{
			if (&((t_event_elem *)curr->content)->button
				== editor->active_event_elem)
			{
				editor->selected_event_elem = curr->content;
				editor->selected_event = editor->selected_event_elem->event;
				break ;
			}
			curr = curr->next;
		}
		if (editor->selected_event)
			set_event_ui(editor, editor->selected_event);
	}
}

/*
 * NOTE:
 * 		Ignore all ui inputs if we have dropdowns open;
*/
void	event_select_events(t_editor *editor)
{
	ui_menu_get_menu(editor->event_menu)->event_children = 1;
	editor->event_scrollbar->event = 1;
	editor->event_sector_input->event = 1;
	editor->event_min_input->event = 1;
	editor->event_max_input->event = 1;
	editor->event_speed_input->event = 1;
	if (ui_dropdown_is_open(editor->event_action_dropdown)
		|| ui_dropdown_is_open(editor->event_type_dropdown)
		|| ui_dropdown_is_open(editor->event_id_dropdown))
	{
		ui_menu_get_menu(editor->event_menu)->event_children = 0;
		editor->event_scrollbar->event = 0;
		editor->event_sector_input->event = 0;
		editor->event_min_input->event = 0;
		editor->event_max_input->event = 0;
		editor->event_speed_input->event = 0;
	}
	event_select(editor);
	event_adding_events(editor);
	event_removing_events(editor);
	event_id_dropdown_update(editor);
}

void	event_events(t_editor *editor)
{
	if (editor->event_button->state == UI_STATE_CLICK)
	{
		editor->event_edit_menu->show = 1;
		event_select_events(editor);
	}
	else
	{
		editor->selected_event = NULL;
		editor->event_edit_menu->show = 0;
	}
}
