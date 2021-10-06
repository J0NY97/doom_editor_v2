#include "libui.h"

void	ui_label_new(t_ui_window *win, t_ui_element *label)
{
	t_ui_label			*lab;

	ui_element_new(win, label);
	label->element = ft_memalloc(sizeof(t_ui_label));
	label->element_type = UI_TYPE_LABEL;
	lab = label->element;
	lab->text = ft_strdup("label text");
	lab->text_align = UI_TEXT_ALIGN_NONE;
	lab->font_path = ft_strdup(UI_FONT_PATH"DroidSans.ttf");
	lab->font_size = 12;
	lab->font_color = 0xff037171;
	lab->max_w = -1;
	lab->font = NULL;
	lab->font_recreate = 1;
	lab->texture_recreate = 1;
}

void	ui_label_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	if (recipe->title)
		ui_label_text_set(elem, recipe->title);
	if (recipe->text_color)
		ui_label_color_set(elem, recipe->text_color);
	if (recipe->text_align)
		ui_label_text_align(elem, recipe->text_align);
}

void	ui_label_texture_redo(t_ui_element *elem)
{
	t_ui_label	*label;

	label = elem->element;
	if (elem->textures[UI_STATE_DEFAULT])
		SDL_FreeSurface(elem->textures[UI_STATE_DEFAULT]);
	if (elem->texture)
		SDL_DestroyTexture(elem->texture);
	elem->textures[UI_STATE_DEFAULT] = ui_surface_create_from_text_recipe(elem->element);	
	if (!elem->textures[UI_STATE_DEFAULT])
	{
		ft_printf("[%s] elem->textures[UI_STATE_DEFAULT] no surface.\n", __FUNCTION__);
		elem->textures[UI_STATE_DEFAULT] = ui_surface_new(1, 1);
	}
	elem->texture = SDL_CreateTextureFromSurface(elem->win->renderer, elem->textures[UI_STATE_DEFAULT]);
	label->texture_recreate = 0;
	elem->pos.w = elem->textures[UI_STATE_DEFAULT]->w;
	elem->pos.h = elem->textures[UI_STATE_DEFAULT]->h;
}

/*
 * Maybe only realign if you have updated the label.
*/
void	ui_label_text_align(t_ui_element *elem, int align)
{
	t_ui_label	*label;

	label = elem->element;
	label->text_align = align;
	if (align && !(align & UI_TEXT_ALIGN_NONE))
	{
		if (align & UI_TEXT_ALIGN_CENTER)
		{
			elem->pos.x = (elem->parent_screen_pos->w / 2) - (label->text_wh.x / 2);
			elem->pos.y = (elem->parent_screen_pos->h / 2) - (label->text_wh.y / 2);
		}
		if (align & UI_TEXT_ALIGN_TOP)
			elem->pos.y = 0;
		if (align & UI_TEXT_ALIGN_BOT)
			elem->pos.y = elem->parent_screen_pos->h - label->text_wh.y;
		if (align & UI_TEXT_ALIGN_LEFT)
			elem->pos.x = 0;
		if (align & UI_TEXT_ALIGN_RIGHT)
			elem->pos.x = elem->parent_screen_pos->w - label->text_wh.x;
	}
}

/*
 * We are only making blank event handler for label,
 * because the ui_layout event handler doesnt like when
 * an element doesnt have one.
*/
void	ui_label_event(t_ui_element *elem, SDL_Event e)
{
	(void)elem;
	(void)e;
}

int	ui_label_render(t_ui_element *elem)
{
	t_ui_label	*label;

	label = elem->element;
	if (!ui_element_render(elem))
		return (0);
	if (label->texture_recreate || elem->win->textures_recreate)
	{
		ui_label_texture_redo(elem);
		ui_label_text_align(elem, label->text_align);
	}
	return (1);
}

/*
 * Every function under this is for editing the label.
*/
void	ui_label_text_set(t_ui_element *label, char *text)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->text)
		ft_strdel(&lab->text);
	lab->text = ft_strdup(text);
	lab->texture_recreate = 1;
}

void	ui_label_font_set(t_ui_element *label, char *font_path)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->font_path)
		ft_strdel(&lab->font_path);
	lab->font_path = ft_strdup(font_path);
	lab->font_recreate = 1;
}

void	ui_label_size_set(t_ui_element *label, size_t size)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->font_size != size)
	{
		lab->font_recreate = 1;
		lab->texture_recreate = 1;
	}
	lab->font_size = size;
}

void	ui_label_color_set(t_ui_element *label, Uint32 color)
{
	t_ui_label	*lab;

	lab = label->element;
	if (lab->font_color != color)
		lab->texture_recreate = 1;
	lab->font_color = color;
}

void	ui_label_text_center(t_ui_element *elem)
{
	t_ui_label		*label;

	label = elem->element;
	if (!label || elem->element_type != UI_TYPE_LABEL || !label->font || !label->text)
	{
		if (!label)
			ft_printf("[ui_label_text_center] No label in element.\n");
		else if (elem->element_type != UI_TYPE_LABEL)
			ft_printf("[ui_label_text_center] Element not of type UI_TYPE_LABEL is %d.\n", elem->element_type);
		else
			ft_printf("[ui_label_text_center] One of the 15 warning you could get... come here and check.\n");
		return ;
	}
	if (elem->parent_type == UI_TYPE_WINDOW)
	{
		elem->pos.x = (((t_ui_window *)elem->parent)->pos.w / 2) - (label->text_wh.x / 2);	
		elem->pos.y = (((t_ui_window *)elem->parent)->pos.h / 2) - (label->text_wh.y / 2);	
	}
	else
	{
		elem->pos.x = (((t_ui_element *)elem->parent)->pos.w / 2) - (label->text_wh.x / 2);	
		elem->pos.y = (((t_ui_element *)elem->parent)->pos.h / 2) - (label->text_wh.y / 2);	
	}
	label->text_align = UI_TEXT_ALIGN_CENTER;
}

/*
 * End of editing functions
*/
void	ui_label_free(void *label)
{
	(void)label;
}

void	ui_label_print(t_ui_label *label)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\ttext : %s\n", label->text);
	ft_printf("\twh : %d %d\n", label->text_wh.x, label->text_wh.y);
	ft_printf("\tfont_path : %s\n", label->font_path);
	ft_printf("\tfont_size : %d\n", label->font_size);
	ft_printf("\tfont_color : %#x\n", label->font_color);
	ft_printf("\tmax_w : %d\n", label->max_w);
	ft_printf("\tfont ? : %d\n", label->font ? 1 : 0);
	ft_printf("\tfont_recreate : %d\n", label->font_recreate);
	ft_printf("\ttexture_recreate : %d\n", label->texture_recreate);
	ft_printf("\ttext_align : %d\n", label->text_align);
}
