#include "editor.h"

t_event	*event_new(void)
{
	t_event	*event;

	event = ft_memalloc(sizeof(t_event));
	return (event);
}

void	event_free(t_event *event)
{
	event->pointer = NULL;
	ft_strdel(&event->sector);
	free(event);
}

void	remove_event_from_list(t_event *event, t_list **list)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (event == curr->content)
		{
			ft_lstdelone_nonfree(list, curr);
			event_free(event);
		}
		curr = curr->next;
	}
}

void	event_elem_free(t_event_elem *elem)
{
	ui_element_free(elem->menu);
	ui_element_free(elem->button);
	ui_element_free(elem->id);
	ui_element_free(elem->type);
	ui_element_free(elem->action);
	ui_element_free(elem->target_id);
	ui_element_free(elem->sector);
	ui_element_free(elem->min);
	ui_element_free(elem->max);
	ui_element_free(elem->speed);
	elem->event = NULL; // This is just a pointer;
	free(elem);
}

void	remove_event_elem_from_list(t_event_elem *elem, t_list **list)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (curr->content == elem)
		{
			event_elem_free(elem);	
			ft_lstdelone_nonfree(list, curr);
		}
		curr = curr->next;
	}
}

t_event_elem	*event_element_new(t_ui_window *win, t_ui_layout *layout, t_ui_element *parent)
{
	t_event_elem	*event_elem;

	event_elem = ft_memalloc(sizeof(t_event_elem));

	event_elem->menu = ft_memalloc(sizeof(t_ui_element));
	ui_menu_new(win, event_elem->menu);
	ui_element_set_parent(event_elem->menu, parent, UI_TYPE_ELEMENT);
	((t_ui_menu *)event_elem->menu->element)->event_and_render_children = 1;
	ui_element_edit(event_elem->menu, ui_list_get_recipe_by_id(layout->recipes, "event_menu_prefab"));

	event_elem->id = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->id);
	ui_element_set_parent(event_elem->id, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->id, ui_list_get_recipe_by_id(layout->recipes, "event_id_prefab"));

	event_elem->type = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->type);
	ui_element_set_parent(event_elem->type, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->type, ui_list_get_recipe_by_id(layout->recipes, "event_type_prefab"));

	event_elem->action = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->action);
	ui_element_set_parent(event_elem->action, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->action, ui_list_get_recipe_by_id(layout->recipes, "event_action_prefab"));

	event_elem->target_id = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->target_id);
	ui_element_set_parent(event_elem->target_id, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->target_id, ui_list_get_recipe_by_id(layout->recipes, "event_target_id_prefab"));

	event_elem->sector = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->sector);
	ui_element_set_parent(event_elem->sector, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->sector, ui_list_get_recipe_by_id(layout->recipes, "event_sector_prefab"));

	event_elem->min = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->min);
	ui_element_set_parent(event_elem->min, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->min, ui_list_get_recipe_by_id(layout->recipes, "event_min_prefab"));

	event_elem->max = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->max);
	ui_element_set_parent(event_elem->max, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->max, ui_list_get_recipe_by_id(layout->recipes, "event_max_prefab"));

	event_elem->speed = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->speed);
	ui_element_set_parent(event_elem->speed, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->speed, ui_list_get_recipe_by_id(layout->recipes, "event_speed_prefab"));

	event_elem->button = ft_memalloc(sizeof(t_ui_element));
	ui_button_new(win, event_elem->button);
	ui_element_set_parent(event_elem->button, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->button, ui_list_get_recipe_by_id(layout->recipes, "event_button_prefab"));
	
	return (event_elem);
}

/*
 * Update the selected event with the selected info on the event edit menu;
*/
void	update_event(t_editor *editor, t_event *event)
{
	int		i;
	char	*active_text; // DONT FREE!

	ft_printf("[%s] Updating event. ", __FUNCTION__);
	// Type
	active_text = ui_button_get_text(ui_dropdown_active(editor->event_type_dropdown));
	i = -1;
	while (++i < EVENT_TYPE_AMOUNT)
		if (ft_strequ(active_text, g_event_type[i]))
			event->type = i;
	// Action
	active_text = ui_button_get_text(ui_dropdown_active(editor->event_action_dropdown));
	i = -1;
	while (++i < EVENT_TYPE_AMOUNT)
		if (ft_strequ(active_text, g_event_action[i]))
			event->action = i;
	// Target
	active_text = ui_button_get_text(ui_dropdown_active(editor->event_id_dropdown));
	if (active_text)
	{
		if (event->action == SECTOR)
		{
			event->pointer_type = TYPE_SECTOR;
			event->pointer = get_sector_by_id_from_list(editor->sectors, ft_atoi(active_text));
		}
		else
		{
			event->pointer_type = TYPE_SPRITE;
			event->pointer = get_sprite_by_id_from_list(editor->sprites, ft_atoi(active_text));
		}
	}

	// Sector
	active_text = ui_input_get_text(editor->event_sector_input);
	if (active_text)
	{
		if (event->sector)
			ft_strdel(&event->sector);
		event->sector = ft_strdup(active_text);
	}
	// Min
	active_text = ui_input_get_text(editor->event_min_input);
	if (active_text)
		event->min = ft_atoi(active_text);
	// Max
	active_text = ui_input_get_text(editor->event_max_input);
	if (active_text)
		event->max = ft_atoi(active_text);
	// Speed
	active_text = ui_input_get_text(editor->event_speed_input);
	if (active_text)
		event->speed = ft_atoi(active_text);
	ft_printf("Success.\n");
}

/*
 * From the event in elem, update all the corresponding elems in elem;
*/
void	update_event_elem(t_event_elem *elem)
{
	char	temp[29];

	ft_printf("[%s] Updating event_elem. ", __FUNCTION__);
	ui_label_set_text(elem->id, ft_b_itoa(elem->event->id, temp));
	ui_label_set_text(elem->type, g_event_type[elem->event->type]);
	ui_label_set_text(elem->action, g_event_action[elem->event->action]);
	if (elem->event->pointer)
	{
		if (elem->event->pointer_type == SECTOR)
			ui_label_set_text(elem->target_id, ft_b_itoa(((t_sector *)elem->event->pointer)->id, temp));
		else
			ui_label_set_text(elem->target_id, ft_b_itoa(((t_sprite *)elem->event->pointer)->id, temp));
	}
	ui_label_set_text(elem->sector, ft_sprintf("%s", elem->event->sector));
	ui_label_set_text(elem->min, ft_b_itoa(elem->event->min, temp));
	ui_label_set_text(elem->max, ft_b_itoa(elem->event->max, temp));
	ui_label_set_text(elem->speed, ft_b_itoa(elem->event->speed, temp));
	ft_printf("Success.\n");
}

int	get_next_event_id(t_list *list)
{
	t_event	*event;
	int			total;
	int			should_be_total;
	int			i;

	total = 0;
	i = 0;
	while (list)
	{
		event = list->content;
		total += event->id;
		i++;
		list = list->next;
	}
	should_be_total = i * (i + 1) / 2;
	return (should_be_total - total);
}
