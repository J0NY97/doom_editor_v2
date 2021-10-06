#include "libui.h"

void	ui_button_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_button	*button;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_button));
	elem->element_type = UI_TYPE_BUTTON;
	button = elem->element;
	button->elem = elem;
	ui_element_color_set(elem, UI_STATE_DEFAULT, 0xff95d7ae);
	ui_element_color_set(elem, UI_STATE_HOVER, 0xff7bae7f);
	ui_element_color_set(elem, UI_STATE_CLICK, 0xff73956f);

	ui_label_new(win, &button->label);
	ui_element_parent_set(&button->label, elem, UI_TYPE_ELEMENT);
	ui_label_text_align(&button->label, UI_TEXT_ALIGN_CENTER);
}

void	ui_button_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_button	*button;

	button = elem->element;
	ui_element_edit(&button->label, recipe);
}

void	ui_button_event(t_ui_element *elem, SDL_Event e)
{
	if (!elem->show)
		return ;
	elem->is_hover = ui_element_is_hover(elem);
	if (elem->is_hover && elem->win->mouse_down)
		elem->is_click = 1;
	if (elem->is_click && !elem->win->mouse_down)
		elem->is_click = 0;
	if (elem->is_click)
		elem->state = UI_STATE_CLICK;
	else if (elem->is_hover)
		elem->state = UI_STATE_HOVER;
	else
		elem->state = UI_STATE_DEFAULT;
	elem->was_click = elem->is_click;
	if (elem->is_toggle == 1)
		elem->state = UI_STATE_CLICK;
}

bool	ui_button(t_ui_element *elem)
{
	if (elem->is_click)
	{
		elem->is_click = 0;
		elem->is_hover = 0;
		return (1);
	}
	return (0);
}

int	ui_button_render(t_ui_element *elem)
{
	t_ui_button	*button;

	if (!ui_element_render(elem))
		return (0);
	button = elem->element;
	ui_label_render(&button->label);
	return (1);
}

/*
 * Returns from button element its child elements (that are specific to this type of element only);
*/
t_ui_element	*ui_button_get(t_ui_element *elem, int ui_type)
{
	if (elem->element_type == UI_TYPE_BUTTON
		&& ui_type == UI_TYPE_LABEL)
		return (ui_button_get_label_element(elem));
	return (NULL);
}

void	ui_button_free(void *button)
{
	(void)button;
}

/*
 * Getters
*/
t_ui_element	*ui_button_get_label_element(t_ui_element *elem)
{
	if (elem->element_type != UI_TYPE_BUTTON)
	{
		ft_printf("[%s] Element is not of type UI_TYPE_BUTTON.", __FUNCTION__);
		return (NULL);
	}
	return (&((t_ui_button *)elem->element)->label);
}

t_ui_label	*ui_button_get_label(t_ui_element *elem)
{
	return (ui_button_get_label_element(elem)->element);
}

