#include "libui.h"

/*
 * NOTE: the reason dropdown has element, is becuase of the parent stuff,
 * 	we dont actually need to show anything thats why the size of it is 0 x 0
*/
void	ui_dropdown_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_dropdown	*drop;

	ui_element_new(win, elem);
	elem->element = ft_memalloc(sizeof(t_ui_dropdown));
	elem->element_type = UI_TYPE_DROPDOWN;
	drop = elem->element;

	ui_element_color_set(elem, UI_STATE_DEFAULT, 0xffff0000);
	ui_element_pos_set(elem, vec4(0, 0, 50, 20));

	ui_label_new(win, &drop->label);
	ui_label_text_set(&drop->label, "Dropdown");

	ui_menu_new(win, &drop->menu);
	ui_element_pos_set(&drop->menu, vec4(0, elem->pos.h, drop->menu.pos.w, drop->menu.pos.h));
	drop->menu.show = 0;
	((t_ui_menu *)drop->menu.element)->event_and_render_children = 1;

	ui_element_parent_set(&drop->label, elem, UI_TYPE_ELEMENT);
	ui_element_parent_set(&drop->menu, elem, UI_TYPE_ELEMENT);
}

void	ui_dropdown_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	ui_element_edit(&drop->label, recipe);
}

void	ui_dropdown_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	ui_checkbox_event(elem, e);
	drop->menu.show = elem->state == UI_STATE_CLICK;
	if (drop->menu.show)
	{
		elem->state = UI_STATE_CLICK;
		ui_menu_event(&drop->menu, e);
		ui_list_radio_event(drop->menu.children, &drop->active, e);
		if (drop->active)
			ui_element_render(drop->active); // because we want it to update.

		{
			// This could be changed to the menu in the same wave as you render or event handle the children.
			t_list *curr;
			t_ui_element *child;
			curr = drop->menu.children;
			int total_height = 0;
			while (curr)
			{
				child = curr->content;
				ui_element_pos_set2(child, vec2(child->pos.x, total_height));
				total_height += child->pos.h;
				curr = curr->next;
			};
			ui_element_pos_set(&drop->menu, vec4(drop->menu.pos.x, drop->menu.pos.y, drop->menu.pos.w, total_height));
		}

	}
	if(e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (elem->is_hover != 1)
		{
			elem->is_click = 0;
			elem->state = UI_STATE_DEFAULT;
			drop->menu.show = 0;
			return ;
		}
	}
}

int	ui_dropdown_render(t_ui_element *elem)
{
	t_ui_dropdown	*drop;

	drop = elem->element;
	if (!ui_button_render(elem))
		return (0);

	ui_element_pos_set(&drop->menu, vec4(0, elem->pos.h, drop->menu.pos.w, drop->menu.pos.h));
	ui_menu_render(&drop->menu);
	return (1);
}

void	ui_dropdown_free(void *drop)
{
	(void)drop;
}

t_ui_element	*ui_dropdown_get(t_ui_element *elem, int ui_type)
{
	if (elem->element_type == UI_TYPE_DROPDOWN)
	{
		if (ui_type == UI_TYPE_MENU)
			return (ui_dropdown_get_menu_element(elem));
	}
	ft_printf("[%s] Something went wrong.\n");
	return (NULL);
}

// Getters

t_ui_element	*ui_dropdown_get_menu_element(t_ui_element *elem)
{
	if (elem->element_type == UI_TYPE_DROPDOWN)
		return (&ui_dropdown_get_dropdown(elem)->menu);
	ft_printf("[%s] Something went wrong.\n");
	return (NULL);
}

/*
 * Returns dropdown from element, if the type is dropdown;
*/
t_ui_dropdown	*ui_dropdown_get_dropdown(t_ui_element *elem)
{
	if (!elem || !elem->element_type == UI_TYPE_DROPDOWN)
		return (NULL);
	return (elem->element);
}
