#ifndef UI_LOAD_H
# define UI_LOAD_H
# include "libui.h"

/*
 * Fun fact: you can only have flexible array in the end of a struct.
 * 	which actually makes sense if you think about it.
*/
typedef struct s_ui_acceptable
{
	char			*name;
	int				type;
	void			(*freer)(void *args);
	void			(*maker)(t_ui_window *win, t_ui_element *elem);
	int				(*renderer)(t_ui_element *elem);
	void			(*eventer)(t_ui_element *elem, SDL_Event e);
	void			(*edit)(t_ui_element *elem, t_ui_recipe *recipe);
	t_ui_element	*(*getter)(t_ui_element *elem, int ui_element_type);
}				t_ui_acceptable;

static const t_ui_acceptable	g_acceptable_element =
{
	.name = "Element",
	.type = UI_TYPE_ELEMENT,
	.freer = NULL,
	.maker = NULL,
	.renderer = &ui_element_render,
	.eventer = &ui_element_event,
	.edit = NULL, //&ui_element_edit,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_button =
{
	.name = "Button",
	.type = UI_TYPE_BUTTON,
	.freer = &ui_button_free,
	.maker = &ui_button_new,
	.renderer = &ui_button_render,
	.eventer = &ui_button_event,
	.edit = &ui_button_edit,
	.getter = NULL //&ui_button_get , button doesnt need this, you can acess the label stuff anyways.
};

static const t_ui_acceptable	g_acceptable_label =
{
	.name = "Label",
	.type = UI_TYPE_LABEL,
	.freer = &ui_label_free,
	.maker = &ui_label_new,
	.renderer = &ui_label_render,
	.eventer = &ui_label_event,
	.edit = &ui_label_edit,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_menu =
{
	.name = "Menu",
	.type = UI_TYPE_MENU,
	.freer = &ui_menu_free,
	.maker = &ui_menu_new,
	.renderer = &ui_menu_render,
	.eventer = &ui_menu_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_dropdown =
{
	.name = "Dropdown",
	.type = UI_TYPE_DROPDOWN,
	.freer = &ui_dropdown_free,
	.maker = &ui_dropdown_new,
	.renderer = &ui_dropdown_render,
	.eventer = &ui_dropdown_event,
	.edit = &ui_dropdown_edit,
	.getter = &ui_dropdown_get
};

static const t_ui_acceptable	g_acceptable_input =
{
	.name = "Input",
	.type = UI_TYPE_INPUT,
	.freer = &ui_input_free,
	.maker = &ui_input_new,
	.renderer = &ui_input_render,
	.eventer = &ui_input_event,
	.edit = &ui_input_edit,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_slider =
{
	.name = "Slider",
	.type = UI_TYPE_SLIDER,
	.freer = &ui_slider_free,
	.maker = &ui_slider_new,
	.renderer = &ui_slider_render,
	.eventer = &ui_slider_event,
	.edit = &ui_slider_edit,
	.getter = &ui_slider_get
};

static const t_ui_acceptable	g_acceptable_checkbox =
{
	.name = "Checkbox",
	.type = UI_TYPE_CHECKBOX,
	.freer = &ui_checkbox_free,
	.maker = &ui_checkbox_new,
	.renderer = &ui_checkbox_render,
	.eventer = &ui_checkbox_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_radio =
{
	.name = "Radio",
	.type = UI_TYPE_RADIO,
	.freer = &ui_radio_free,
	.maker = &ui_radio_new,
	.renderer = &ui_radio_render,
	.eventer = &ui_radio_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_tab =
{
	.name = "Tab",
	.type = UI_TYPE_TAB,
	.freer = &ui_tab_free,
	.maker = &ui_tab_new,
	.renderer = &ui_tab_render,
	.eventer = &ui_tab_event,
	.edit = NULL,
	.getter = NULL
};

static const t_ui_acceptable	g_acceptable_window =
{
	.name = "Window",
	.type = UI_TYPE_WINDOW,
	.freer = NULL,
	.maker = NULL,
	.renderer = NULL,
	.eventer = NULL,
	.edit = NULL,
	.getter = NULL
};

// NOTE: these must be in the same order as the UI_TYPE_ENUM
static const t_ui_acceptable	g_acceptable[] =
{
	g_acceptable_element,
	g_acceptable_label,
	g_acceptable_button,
	g_acceptable_menu,
	g_acceptable_dropdown,
	g_acceptable_input,
	g_acceptable_slider,
	g_acceptable_checkbox,
	g_acceptable_radio,
	g_acceptable_tab,
	g_acceptable_window,
	NULL
};

#endif
