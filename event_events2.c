/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_events2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/18 12:53:55 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/18 12:54:09 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	event_removing_events(t_editor *editor)
{
	if (editor->remove_event_button->state == UI_STATE_CLICK
		&& editor->selected_event && editor->selected_event_elem)
	{
		remove_event(editor, editor->selected_event);
		editor->selected_event_elem = NULL;
		editor->active_event_elem = NULL;
		editor->selected_event = NULL;
		if (editor->event_amount == 0)
			((t_ui_scrollbar *)editor->event_scrollbar->element)->bot_most
				= vec2i(0, 0);
		((t_ui_scrollbar *)editor->event_scrollbar->element)->update = 1;
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
