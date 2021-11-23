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

t_event	*add_event(t_editor *editor)
{
	t_event	*event;

	event = event_new();
	event->elem = event_element_new(editor->win_main, &editor->layout, editor->event_menu);
	event->id = get_next_event_id(editor->events);
	event->elem->event = event;

	t_vec2	new_pos;
	new_pos = vec2(event->elem->menu->pos.x, ((t_ui_scrollbar *)editor->event_scrollbar->element)->bot_most.y + 10);
	ui_element_pos_set2(event->elem->menu, new_pos);
	
	add_to_list(&editor->event_elements, event->elem, sizeof(t_event_elem));
	add_to_list(&editor->event_element_buttons, event->elem->button, sizeof(t_ui_element));
	add_to_list(&editor->events, event, sizeof(t_event));

	++editor->event_amount;

	ft_printf("New event added (%d)\n", editor->event_amount);
	return (event);
}

void	remove_event(t_editor *editor, t_event *event)
{
	ft_printf("[%s] Removing event.\n", __FUNCTION__);
	ui_element_remove_from_list(event->elem->button, &editor->event_element_buttons);
	remove_event_elem_from_list(event->elem, &editor->event_elements);
	remove_event_from_list(event, &editor->events);
	ft_printf("If you see ui_element_remove_child_parent, dont get scared, those are coming from the ui_element_free.\n");
	event_elem_free(event->elem);
	event_free(event);
	--editor->event_amount;
	//<---- HERE!!! TODO: realign all the elements on event_id_menu;
	ft_printf("Removing event. (total : %d)\n", editor->event_amount);
}

void	set_event_ui(t_editor *editor, t_event *event)
{
	char	target_id_text[20];

	ft_strnclr(target_id_text, 20);

	ui_dropdown_activate(editor->event_type_dropdown,
		ui_list_get_button_with_text(ui_dropdown_get_menu_element(editor->event_type_dropdown)->children,
			g_event_type[event->type]));
	ui_dropdown_activate(editor->event_action_dropdown,
		ui_list_get_button_with_text(ui_dropdown_get_menu_element(editor->event_action_dropdown)->children,
			g_event_action[event->action]));

	if (event->pointer)
	{
		if (event->pointer_type == TYPE_SECTOR)
			ft_b_itoa(((t_sector *)event->pointer)->id, target_id_text);
		else
			ft_b_itoa(((t_sprite *)event->pointer)->id, target_id_text);
	}
	// Activate ID button;
	t_ui_element	*id_button = ui_list_get_button_with_text(
			ui_dropdown_get_menu_element(editor->event_id_dropdown)->children, target_id_text);
	if (!id_button)
	{
		if (event->pointer_type == TYPE_SECTOR
			|| event->pointer_type == SECTOR)
		{
			char	**texts = gen_sector_id_texts(editor->sectors);
			t_ui_recipe *recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "event_id_button"); // TODO: save this recipe in the editor;
			create_buttons_to_list_from_texts_remove_extra(ui_dropdown_get_menu_element(editor->event_id_dropdown), texts, recipe);
			ft_arraydel(texts);
		}
		id_button = ui_list_get_button_with_text(
				ui_dropdown_get_menu_element(editor->event_id_dropdown)->children, target_id_text);
	}
	ui_dropdown_activate(editor->event_id_dropdown, id_button);
}

void	remove_event_from_list(t_event *event, t_list **list)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (event == curr->content)
			ft_lstdelone_nonfree(list, curr);
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
			ft_lstdelone_nonfree(list, curr);
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
	((t_ui_menu *)event_elem->menu->element)->event_children = 1;
	((t_ui_menu *)event_elem->menu->element)->render_children = 1;
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

	ft_printf("[%s] Event elem created.\n", __FUNCTION__);
	
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
		if (elem->event->pointer_type == TYPE_SECTOR)
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
