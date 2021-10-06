#include "libui.h"

void	ui_checkbox_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_checkbox	*checkbox;

	ui_element_new(win, elem);
	checkbox = ft_memalloc(sizeof(t_ui_checkbox));
	elem->element = checkbox;
	elem->element_type = UI_TYPE_CHECKBOX;
}

/*
 * NOTE for DEV:
 * try to keep this element only,
 * because its used in the radio buttons.
*/
void	ui_checkbox_event(t_ui_element *elem, SDL_Event e)
{
	elem->is_hover = 0;
	if (point_in_rect(elem->win->mouse_pos, elem->screen_pos))
		elem->is_hover = 1;
	if (elem->is_hover && e.type == SDL_MOUSEBUTTONDOWN)
		elem->is_click = elem->is_click == 0;
	if (elem->is_click)
		elem->state = UI_STATE_CLICK;
	else if (elem->is_hover)
		elem->state = UI_STATE_HOVER;
	else
		elem->state = UI_STATE_DEFAULT;
}

int	ui_checkbox_render(t_ui_element *elem)
{
	ui_element_render(elem);
}

void	ui_checkbox_free(void *elem)
{
	(void)elem;
}
