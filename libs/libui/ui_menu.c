#include "libui.h"

void	ui_menu_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_menu	*menu;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_menu));
	elem->element_type = UI_TYPE_MENU;
	menu = elem->element;
	menu->elem = elem;
}

void	ui_menu_event(t_ui_element *elem, SDL_Event e)
{
	t_list			*curr;
	t_ui_element	*child;
	t_ui_menu		*menu;

	menu = elem->element;
	if (!menu->event_and_render_children)
		return ;
	curr = elem->children;
	while (curr)
	{
		child = curr->content;
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			if (child->element_type >= 0
				&& child->element_type < UI_TYPE_AMOUNT
				&& g_acceptable[child->element_type].eventer)
				g_acceptable[child->element_type].eventer(child, e);
			else
				ft_printf("[%s] {%s} : Eventing of type %d %d is not supported.\n", __FUNCTION__, elem->id, curr->content_size, elem->element_type);
		}
		else
			ft_printf("[%s] Element [%s] @ [%d, %d] of type %d is not supported.\n", __FUNCTION__, child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size);
		curr = curr->next;
	}
}

int	ui_menu_render(t_ui_element *elem)
{
	t_list			*curr;
	t_ui_menu		*menu;
	t_ui_element	*child;

	if (!ui_element_render(elem))
		return (0);
	menu = elem->element;
	if (!menu->event_and_render_children)
		return (1);
	curr = elem->children;
	while (curr)
	{
		if (curr->content_size == UI_TYPE_ELEMENT)
		{
			child = curr->content;
			if (child->element_type >= 0
				&& child->element_type < UI_TYPE_AMOUNT
				&& g_acceptable[child->element_type].renderer)
				g_acceptable[child->element_type].renderer(child);
			else
				ft_printf("[%s] {%s} : Rendering \"%s\" of type ([%d] == 0?) [%d] %s is not supported.\n", __FUNCTION__, elem->id, child->id, curr->content_size, child->element_type, ui_element_type_to_string(child->element_type));
		}
		else
			ft_printf("[%s] Element isnt UI_TYPE_ELEMENT ... HOW? ... [%s] @ [%d, %d] of type %d is not supported.\n", __FUNCTION__, child->screen_pos.x, child->screen_pos.y, child->id, curr->content_size);
		curr = curr->next;
	}
	return (2);
}

void	ui_menu_free(void *menu)
{
	(void)menu;
}

// Gettesr

t_ui_menu	*ui_menu_get_menu(t_ui_element *elem)
{
	if (elem->element_type == UI_TYPE_MENU)
		return (elem->element);
	return (NULL);
}
