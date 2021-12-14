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

/*
 * Try to activate button with the 'target_id_text',
 * 	if it doesnt exist we update the list,
 * 	and try again.
*/
/*
 * TODO: we have problem here,
 * if you update id drop with more sectors than sprites,
 * and then open sprite ids with less buttons;
*/
void	activate_id_button(t_editor *editor, int type, char *target_id_text)
{
	t_ui_element	*id_button;
	t_ui_recipe		*recipe;
	char			**texts;

	id_button = ui_dropdown_get_button_with_text(
			editor->event_id_dropdown, target_id_text);
	if (!id_button)
	{
		if (type == TYPE_SECTOR)
			texts = gen_sector_id_texts(editor->sectors);
		else
			texts = gen_sprite_id_texts(editor->sprites);
		ft_printf("[%s] Creating new id buttons.\n", __FUNCTION__);
		recipe = ui_layout_get_recipe(&editor->layout, "event_id_button");
		create_buttons_to_list_from_texts_remove_extra(
			ui_dropdown_get_menu_element(editor->event_id_dropdown),
			texts, recipe);
		ft_printf("[%s] Done creating id buttons.\n", __FUNCTION__);
		ft_arraydel(texts);
		id_button = ui_dropdown_get_button_with_text(
				editor->event_id_dropdown, target_id_text);
	}
	ui_dropdown_activate(editor->event_id_dropdown, id_button);
}

void	set_event_ui(t_editor *editor, t_event *event)
{
	char	temp_str[20];

	ui_dropdown_activate(editor->event_type_dropdown,
		ui_dropdown_get_button_with_text(editor->event_type_dropdown,
			(char *)g_event_type[event->type]));
	ui_dropdown_activate(editor->event_action_dropdown,
		ui_dropdown_get_button_with_text(editor->event_action_dropdown,
			(char *)g_event_action[event->action]));
	if (event->pointer)
	{
		if (event->pointer_type == TYPE_SECTOR)
			ft_b_itoa(((t_sector *)event->pointer)->id, temp_str);
		else
			ft_b_itoa(((t_sprite *)event->pointer)->id, temp_str);
		activate_id_button(editor, event->pointer_type, temp_str);
	}
	if (event->sector)
		ui_input_set_text(editor->event_sector_input, event->sector);
	ui_input_set_text(editor->event_min_input, ft_b_itoa(event->min, temp_str));
	ui_input_set_text(editor->event_max_input, ft_b_itoa(event->max, temp_str));
	ui_input_set_text(editor->event_speed_input,
		ft_b_itoa(event->speed, temp_str));
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
 * Update the selected event with the selected info on the event edit menu;
 * NOTE: this is the first input getter function we made,
 * 		but this is basically 'get_event_ui';
 *
 * char	*active_text; // DONT FREE!
*/
void	update_event2(t_editor *editor, t_event *event)
{
	char	*active_text;
	int		id;

	active_text = ui_dropdown_active_text(editor->event_id_dropdown);
	if (active_text)
	{
		id = ft_atoi(active_text);
		if (event->action == SECTOR)
		{
			event->pointer_type = TYPE_SECTOR;
			event->pointer = get_sector_by_id_from_list(editor->sectors, id);
		}
		else
		{
			event->pointer_type = TYPE_SPRITE;
			event->pointer = get_sprite_by_id_from_list(editor->sprites, id);
		}
	}
}

void	update_event4(t_editor *editor, t_event *event)
{
	char	*active_text;

	active_text = ui_input_get_text(editor->event_sector_input);
	if (active_text)
	{
		if (event->sector)
			ft_strdel(&event->sector);
		if (ft_strequ(active_text, ""))
			event->sector = ft_strdup("-");
		else
			event->sector = ft_strdup(active_text);
	}
}

/*
 * char	*active_text; is pointer to an existing one, DONT FREE!
*/
void	update_event3(t_editor *editor, t_event *event)
{
	char	*active_text;

	active_text = ui_input_get_text(editor->event_min_input);
	if (active_text)
		event->min = ft_atoi(active_text);
	active_text = ui_input_get_text(editor->event_max_input);
	if (active_text)
		event->max = ft_atoi(active_text);
	active_text = ui_input_get_text(editor->event_speed_input);
	if (active_text)
		event->speed = ft_atoi(active_text);
}

void	update_event(t_editor *editor, t_event *event)
{
	int		i;
	char	*active_text;

	active_text = ui_dropdown_active_text(editor->event_type_dropdown);
	i = -1;
	while (++i < EVENT_TYPE_AMOUNT)
		if (ft_strequ(active_text, g_event_type[i]))
			event->type = i;
	active_text = ui_dropdown_active_text(editor->event_action_dropdown);
	i = -1;
	while (++i < EVENT_ACTION_AMOUNT)
		if (ft_strequ(active_text, g_event_action[i]))
			event->action = i;
	update_event2(editor, event);
	update_event3(editor, event);
	update_event4(editor, event);
}

/*
 * From the event in elem, update all the corresponding elems in elem;
*/
void	update_event_elem(t_event_elem *elem)
{
	char	temp[29];

	ft_printf("[%s] Updating event_elem. ", __FUNCTION__);
	ui_label_set_text(&elem->id, ft_b_itoa(elem->event->id, temp));
	ui_label_set_text(&elem->type, (char *)g_event_type[elem->event->type]);
	ui_label_set_text(&elem->action,
		(char *)g_event_action[elem->event->action]);
	if (elem->event->pointer)
	{
		if (elem->event->pointer_type == TYPE_SECTOR)
			ui_label_set_text(&elem->target_id,
				ft_b_itoa(((t_sector *)elem->event->pointer)->id, temp));
		else
			ui_label_set_text(&elem->target_id,
				ft_b_itoa(((t_sprite *)elem->event->pointer)->id, temp));
	}
	ui_label_set_text(&elem->sector, elem->event->sector);
	ui_label_set_text(&elem->min, ft_b_itoa(elem->event->min, temp));
	ui_label_set_text(&elem->max, ft_b_itoa(elem->event->max, temp));
	ui_label_set_text(&elem->speed, ft_b_itoa(elem->event->speed, temp));
	ft_printf("Success.\n");
}

int	get_next_event_id(t_list *list)
{
	t_event		*event;
	int			i;
	char		ids[1024];

	i = -1;
	memset(ids, 0, sizeof(char) * 1024);
	while (list)
	{
		event = list->content;
		ids[event->id] = 1;
		list = list->next;
	}
	while (ids[++i])
		;
	return (i);
}

int	get_event_type(char *type_name)
{
	int	ddd;

	ddd = -1;
	while (++ddd < EVENT_TYPE_AMOUNT)
		if (ft_strequ(type_name, g_event_type[ddd]))
			return (ddd);
	return (0);
}

int	get_event_action(char *type_name)
{
	int	ddd;

	ddd = -1;
	while (++ddd < EVENT_ACTION_AMOUNT)
		if (ft_strequ(type_name, g_event_action[ddd]))
			return (ddd);
	return (0);
}
