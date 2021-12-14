/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_elem.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:30:31 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:30:31 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_event_elem	*event_element_new(t_ui_element *parent)
{
	t_event_elem	*event_elem;

	event_elem = ft_memalloc(sizeof(t_event_elem));
	set_elem_parent_and_recipe(&event_elem->menu,
		UI_TYPE_MENU, parent, "event_menu_prefab");
	set_elem_parent_and_recipe(&event_elem->id,
		UI_TYPE_LABEL, &event_elem->menu, "event_id_prefab");
	set_elem_parent_and_recipe(&event_elem->type,
		UI_TYPE_LABEL, &event_elem->menu, "event_type_prefab");
	set_elem_parent_and_recipe(&event_elem->action,
		UI_TYPE_LABEL, &event_elem->menu, "event_action_prefab");
	set_elem_parent_and_recipe(&event_elem->target_id,
		UI_TYPE_LABEL, &event_elem->menu, "event_target_id_prefab");
	set_elem_parent_and_recipe(&event_elem->sector,
		UI_TYPE_LABEL, &event_elem->menu, "event_sector_prefab");
	set_elem_parent_and_recipe(&event_elem->min,
		UI_TYPE_LABEL, &event_elem->menu, "event_min_prefab");
	set_elem_parent_and_recipe(&event_elem->max,
		UI_TYPE_LABEL, &event_elem->menu, "event_max_prefab");
	set_elem_parent_and_recipe(&event_elem->speed,
		UI_TYPE_LABEL, &event_elem->menu, "event_speed_prefab");
	set_elem_parent_and_recipe(&event_elem->button,
		UI_TYPE_BUTTON, &event_elem->menu, "event_button_prefab");
	return (event_elem);
}

/*
 * NOTE:
 * 1.	When freeing an element, it will also free all its children,
 * 	and since all the event_elem elements are a child of 't_event_elem->menu'
 * 	all of them will be freed when that one is freed.
 * 2.	We dont free it here because this function is also used in the final
 * 	freeing, so you have to free the menu before freeing the 't_event_elem'.
*/
void	event_elem_free(void *e_e, size_t size)
{
	t_event_elem	*elem;

	elem = e_e;
	if (!elem)
		return ;
	free(elem);
	(void)size;
}

void	realign_event_buttons(t_editor *editor)
{
	t_list			*curr;
	t_event_elem	*event_elem;
	int				new_val;
	int				h;
	int				i;

	curr = editor->event_elements;
	i = 0;
	h = 60;
	while (curr && curr->next)
		curr = curr->next;
	while (curr)
	{
		event_elem = curr->content;
		ui_element_pos_set2(&event_elem->menu, vec2(event_elem->menu.pos.x,
				((t_ui_scrollbar *)editor->event_scrollbar->element)->top_most.y
				+ (event_elem->menu.pos.h * i) + (i * 10)));
		i++;
		curr = curr->prev;
	}
	new_val = ft_max(((t_ui_scrollbar *)editor->event_scrollbar->element)->value
			- h - 10, 0);
	ui_scroll_value_set(editor->event_scrollbar, new_val);
	((t_ui_scrollbar *)editor->event_scrollbar->element)->update = 1;
}
