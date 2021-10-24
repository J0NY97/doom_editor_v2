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
	t_ui_element	*button;
	t_ui_element	*info;
	t_ui_recipe		*button_recipe;
	t_ui_recipe		*info_recipe;

	button_recipe = ui_list_get_recipe_by_id(layout->recipes, "event_button_prefab");
	info_recipe = ui_list_get_recipe_by_id(layout->recipes, "event_info_prefab");

	event_elem = ft_memalloc(sizeof(t_event_elem));
	
	button = ft_memalloc(sizeof(t_ui_element));
	ui_button_new(win, button);
	ui_element_parent_set(button, parent, UI_TYPE_ELEMENT);
	ui_element_edit(button, button_recipe);

	info = ft_memalloc(sizeof(t_ui_element));
	ui_label_new(win, info);
	info->z = 1;
	ui_element_parent_set(info, button, UI_TYPE_ELEMENT);
	ui_element_edit(info, info_recipe);

	event_elem->button = button;
	event_elem->info = info;
	return (event_elem);
}

void	event_elem_fill(t_editor *editor, t_event_elem *event_elem)
{
	char	temp[20];
	char	*temp2;

	temp2 = ft_strjoin("event_", ft_b_itoa(editor->event_amount++, temp));
	ui_label_text_set(ui_button_get_label_element(event_elem->button), temp2);
	ft_strdel(&temp2);

	t_vec2	new_pos;
	/*
	new_pos = vec2(event_elem->button->pos.x,
		event_elem->button->pos.h * (editor->event_amount - 1) + (editor->event_amount - 1) * 10);
		*/
	new_pos = vec2(event_elem->button->pos.x, ((t_ui_scrollbar *)editor->event_scrollbar->element)->bot_most.y + 10);
	ui_element_pos_set2(event_elem->button, new_pos);

	ui_label_text_set(event_elem->info, "just showing random text to test if this is working");
}

/*
 * int	event_type;			one of the types in temp.h from event_types enum;
*/
void	fill_event_id_dropdown(t_editor *editor, int event_type)
{
	t_ui_recipe	*drop_button_recipe;

	drop_button_recipe = ui_list_get_recipe_by_id(editor->layout.recipes, "event_drop_button_prefab");
}
