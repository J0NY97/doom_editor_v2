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
	ui_element_free(elem->info);
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
	t_ui_recipe		*menu_recipe;
	t_ui_recipe		*button_recipe;
	t_ui_recipe		*info_recipe;

	menu_recipe = ui_list_get_recipe_by_id(layout->recipes, "event_menu_prefab");
	button_recipe = ui_list_get_recipe_by_id(layout->recipes, "event_button_prefab");
	info_recipe = ui_list_get_recipe_by_id(layout->recipes, "event_info_prefab");

	event_elem = ft_memalloc(sizeof(t_event_elem));

	event_elem->menu = ft_memalloc(sizeof(t_ui_element));
	ui_menu_new(win, event_elem->menu);
	ui_element_set_parent(event_elem->menu, parent, UI_TYPE_ELEMENT);
	((t_ui_menu *)event_elem->menu->element)->event_and_render_children = 1;
	ui_element_edit(event_elem->menu, menu_recipe);

	event_elem->info = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->info);
	ui_element_set_parent(event_elem->info, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->info, info_recipe);
	ui_element_set_id(event_elem->info, "event_info");
	
	event_elem->button = ft_memalloc(sizeof(t_ui_element));
	ui_button_new(win, event_elem->button);
	ui_element_set_parent(event_elem->button, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->button, button_recipe);

	return (event_elem);
}

void	event_elem_update(t_editor *editor, t_event_elem *event_elem)
{
	char	temp[20];
	char	*temp2;
	char	*final_info;

	if (!event_elem || !event_elem->event)
		return ;
	final_info = NULL;
	int		i;
	char	*tem;
	// Type
	i = -1;
	tem = ui_dropdown_active_text(editor->event_type_dropdown);
	while (++i < EVENT_TYPE_AMOUNT && tem)
	{
		if (ft_strequ(g_event_type[i], tem))
		{
			event_elem->event->type = i;
			break ;
		}
	}
	// Action
	i = -1;
	tem = ui_dropdown_active_text(editor->event_action_dropdown);
	while (++i < EVENT_ACTION_AMOUNT && tem)
	{
		if (ft_strequ(g_event_action[i], tem))
		{
			event_elem->event->action = i;
			break ;
		}
	}

	final_info = ft_sprintf("%s, %s", g_event_type[event_elem->event->type], g_event_action[event_elem->event->action]);
	ui_label_set_text(event_elem->info, final_info);
	ft_strdel(&final_info);
}

/*
 * int	event_type;			one of the types in temp.h from event_types enum;
*/
void	fill_event_id_dropdown(t_editor *editor, int event_type)
{
	t_ui_recipe	*drop_button_recipe;

	drop_button_recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "event_drop_button_prefab");
}
