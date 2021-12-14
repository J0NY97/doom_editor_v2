/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:03:49 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 13:43:31 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_event	*event_new(void)
{
	t_event	*event;

	event = ft_memalloc(sizeof(t_event));
	return (event);
}

void	event_free(void *ev, size_t size)
{
	t_event	*event;

	event = ev;
	if (!event)
		return ;
	event->pointer = NULL;
	ft_strdel(&event->sector);
	free(event);
	(void)size;
}

t_event	*add_event(t_editor *editor)
{
	t_event	*event;
	t_vec2	new_pos;

	event = event_new();
	event->elem = event_element_new(editor->event_menu);
	event->id = get_next_event_id(editor->events);
	event->elem->event = event;
	new_pos = vec2(event->elem->menu.pos.x,
			((t_ui_scrollbar *)editor->event_scrollbar->element)->bot_most.y
			+ 10);
	ui_element_pos_set2(&event->elem->menu, new_pos);
	add_to_list(&editor->event_elements, event->elem, sizeof(t_event_elem));
	add_to_list(&editor->event_element_buttons, &event->elem->button,
		sizeof(t_ui_element));
	add_to_list(&editor->events, event, sizeof(t_event));
	++editor->event_amount;
	ft_printf("New event added (%d)\n", editor->event_amount);
	ui_scrollbar_event(editor->event_scrollbar, (SDL_Event){});
	ui_scrollbar_render(editor->event_scrollbar);
	ui_scroll_value_set(editor->event_scrollbar,
		((t_ui_scrollbar *)editor->event_scrollbar->element)->max);
	return (event);
}

/*
 * ft_printf("If you see ui_element_remove_child_parent,
 * 	dont get scared, those are coming from the ui_element_free.\n");
*/
void	remove_event(t_editor *editor, t_event *event)
{
	ft_printf("[%s] Removing event.\n", __FUNCTION__);
	remove_from_list(&editor->event_element_buttons, &event->elem->button);
	remove_from_list(&editor->event_elements, event->elem);
	remove_from_list(&editor->events, event);
	ui_element_free(&event->elem->menu, event->elem->menu.element_type);
	event_elem_free(event->elem, sizeof(t_event_elem));
	event_free(event, sizeof(t_event));
	--editor->event_amount;
	realign_event_buttons(editor);
	ft_printf("Removing event. (total : %d)\n", editor->event_amount);
}
