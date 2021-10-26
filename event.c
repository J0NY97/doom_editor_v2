#include "editor.h"

t_event	*event_new(void)
{
	t_event	*event;

	event = ft_memalloc(sizeof(t_event));
	return (event);
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
	ui_element_parent_set(event_elem->menu, parent, UI_TYPE_ELEMENT);
	((t_ui_menu *)event_elem->menu->element)->event_and_render_children = 1;
	ui_element_edit(event_elem->menu, menu_recipe);

	event_elem->info = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, event_elem->info);
	ui_element_parent_set(event_elem->info, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->info, info_recipe);
	ui_element_set_id(event_elem->info, "event_info");
	
	event_elem->button = ft_memalloc(sizeof(t_ui_element));
	ui_button_new(win, event_elem->button);
	ui_element_parent_set(event_elem->button, event_elem->menu, UI_TYPE_ELEMENT);
	ui_element_edit(event_elem->button, button_recipe);

	return (event_elem);
}

void	event_elem_update(t_editor *editor, t_event_elem *event_elem)
{
	char	temp[20];
	char	*temp2;
	char	*final_info;

	if (!event_elem)
		return ;
	final_info = NULL;
	/*
	final_info = ft_sprintf("%s, %s, %s, %s, %s, %s",
			ui_dropdown_active_text(editor->event_type_dropdown),
			ui_dropdown_active_text(editor->event_action_dropdown),
			ui_dropdown_active_text(editor->event_id_dropdown),
			ui_input_get_text(editor->event_sector_input),
			ui_input_get_text(editor->event_min_input),
			ui_input_get_text(editor->event_max_input)
		);
		*/
	ft_printf("Event:\n");
	if (!event_elem->event)
	{
		ft_printf("[%s] For some reason we dont have an event.\n", __FUNCTION__);
		return ;
	}
	ft_printf("do we even have an event ? : %d\n", event_elem->event ? 1 : 0);
	ft_printf("the num value of the type is : %d\n", event_elem->event->type);
	ft_printf("we want to put in the event info : %s\n", g_event_type[event_elem->event->type]);
	ft_stradd(&final_info, g_event_type[event_elem->event->type]);
	ui_label_text_set(event_elem->info, final_info);
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
