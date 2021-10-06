#include "libui.h"

void	ui_tab_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_tab	*tab;

	ui_element_new(win, elem);
	tab = ft_memalloc(sizeof(t_ui_tab));
	elem->element = tab;
	elem->element_type = UI_TYPE_TAB;
	tab->elem = elem;
	tab->active = NULL;
	tab->active_menu = NULL;
	tab->buttons = NULL;
	tab->menus = NULL;
}

void	ui_tab_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_element	*button_elem;
	t_ui_element	*menu_elem;
	t_ui_tab		*tab;
	t_list			*curr_button;
	t_list			*curr_menu;

	if (!elem->show || !*elem->parent_show)
		return ;
	tab = elem->element;
	curr_button = tab->buttons;
	curr_menu = tab->menus;
	while (curr_button)
	{
		button_elem = curr_button->content;
		menu_elem = curr_menu->content;
		ui_button_event(button_elem, e);
		ui_menu_event(menu_elem, e);
		if (button_elem->is_click)
		{
			tab->active = button_elem;
			tab->active_menu = menu_elem;
		}
		menu_elem->show = 0;
		button_elem->state = UI_STATE_DEFAULT;
		curr_button = curr_button->next;
		curr_menu = curr_menu->next;
	}
	if (tab->active)
	{
		tab->active->state = UI_STATE_CLICK;
		tab->active_menu->show = 1;
	}
}

int	ui_tab_render(t_ui_element *elem)
{
	t_ui_tab	*tab;
	t_list		*curr_button;
	t_list		*curr_menu;

	if (!ui_element_render(elem))
		return (0);
	tab = elem->element;
	curr_button = tab->buttons;
	curr_menu = tab->menus;
	while (curr_button)
	{
		ui_button_render(curr_button->content);
		ui_menu_render(curr_menu->content);
		curr_button = curr_button->next;
		curr_menu = curr_menu->next;
	}
	return (1);
}

void	ui_tab_free(void *elem)
{
	(void)elem;
}

/*
 * Other functions.
*/

/*
 * NOTE: this function makes the elem the parent of the button and menu.
*/
void	ui_tab_add(t_ui_element *elem, t_ui_element *button, t_ui_element *menu)
{
	t_ui_tab	*tab;

	tab = elem->element;
	ui_element_parent_set(button, elem, UI_TYPE_ELEMENT);
	ui_element_parent_set(menu, elem, UI_TYPE_ELEMENT);
	add_to_list(&tab->buttons, button, UI_TYPE_ELEMENT);
	add_to_list(&tab->menus, menu, UI_TYPE_ELEMENT);
}
