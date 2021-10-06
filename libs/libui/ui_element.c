#include "libui.h"

/*
 * All elements should be in stack memory,
 * and you should pass in the address of it to this function,
 * which then reset the values to the default values.
 *
 * NOTE: if you resize the element, you have to update the texture.
*/
void	ui_element_new(t_ui_window *win, t_ui_element *elem)
{
	memset(elem, 0, sizeof(t_ui_element));
	elem->win = win;
	elem->pos = vec4(0, 0, 50, 20);
	elem->state = UI_STATE_DEFAULT;
	elem->colors[UI_STATE_DEFAULT] = 0xff95D7AE;
	elem->colors[UI_STATE_HOVER] = 0xff7BAE7F;
	elem->colors[UI_STATE_CLICK] = 0xff73956F;
	elem->images[UI_STATE_DEFAULT] = NULL;
	elem->images[UI_STATE_HOVER] = NULL;
	elem->images[UI_STATE_CLICK] = NULL;
	elem->use_images = 0;
	elem->texture_recreate = 1;
	ui_element_textures_redo(elem);
	ui_element_parent_set(elem, win, UI_TYPE_WINDOW);
	elem->element = NULL;
	elem->element_type = UI_TYPE_ELEMENT;
	elem->show = 1;
	elem->last_state = -999;
	elem->children = NULL;
}

/*
 * We have this only so that we wouldnt have errors spamming the terminal.
*/
void	ui_element_event(t_ui_element *elem, SDL_Event e)
{
	(void)elem;
	(void)e;
}

/*
 * Redos the textures, helpful if you have changed the w and/or h of the element.
*/
void	ui_element_textures_redo(t_ui_element *elem)
{
	int	i;

	i = -1;
	while (++i < UI_STATE_AMOUNT)
	{
		if (elem->textures[i])
			SDL_FreeSurface(elem->textures[i]);
	}
	elem->textures[UI_STATE_DEFAULT] = ui_surface_new(elem->pos.w, elem->pos.h);
	elem->textures[UI_STATE_HOVER] = ui_surface_new(elem->pos.w, elem->pos.h);
	elem->textures[UI_STATE_CLICK] = ui_surface_new(elem->pos.w, elem->pos.h);
	elem->texture = SDL_CreateTextureFromSurface(elem->win->renderer, elem->textures[elem->state]);
	elem->texture_recreate = 0;
	if (elem->use_images)
	{
		i = -1;
		while (++i < UI_STATE_AMOUNT)
		{
			if (!elem->images[i])
				continue ;
			SDL_BlitScaled(elem->images[i], NULL, elem->textures[i], NULL);
		}
		return ;
	}
	ui_surface_fill(elem->textures[UI_STATE_DEFAULT], elem->colors[UI_STATE_DEFAULT]);
	ui_surface_fill(elem->textures[UI_STATE_HOVER], elem->colors[UI_STATE_HOVER]);
	ui_surface_fill(elem->textures[UI_STATE_CLICK], elem->colors[UI_STATE_CLICK]);
}

/*
 * Return: 1 if rendering successful, 0 if not.
*/
int	ui_element_render(t_ui_element *elem)
{
	if (!*elem->parent_show || !elem->show)
		return (0);
	if (!elem->textures[elem->state])
		return (0);
	elem->screen_pos = ui_element_screen_pos_get(elem);
	if (elem->texture_recreate || elem->win->textures_recreate)
	{
		ui_element_textures_redo(elem);
		SDL_UpdateTexture(elem->texture, NULL, elem->textures[elem->state]->pixels, elem->textures[elem->state]->pitch);
	}
	else if (elem->state != elem->last_state)
		SDL_UpdateTexture(elem->texture, NULL, elem->textures[elem->state]->pixels, elem->textures[elem->state]->pitch);
	SDL_SetRenderTarget(elem->win->renderer, elem->win->texture);
	SDL_RenderCopy(elem->win->renderer, elem->texture, NULL,
		&(SDL_Rect){elem->screen_pos.x, elem->screen_pos.y, elem->screen_pos.w, elem->screen_pos.h});
	elem->last_state = elem->state;
	return (1);
}

int	ui_element_is_hover(t_ui_element *elem)
{
	if (point_in_rect(elem->win->mouse_pos, elem->screen_pos))
		return (1);
	return (0);
}

int	ui_element_is_click(t_ui_element *elem)
{
	if (ui_element_is_hover(elem) && elem->win->mouse_down)
		return (1);
	return (0);
}

/*
 * Editing functions after this.
*/

t_vec4i	ui_element_screen_pos_get(t_ui_element *elem)
{
	t_vec4i	screen_pos;

	screen_pos.w = elem->pos.w;
	screen_pos.h = elem->pos.h;
	screen_pos.x = elem->parent_screen_pos->x + elem->pos.x;
	screen_pos.y = elem->parent_screen_pos->y + elem->pos.y;
	return (screen_pos);
}

/*
 * You can either give in absolute values,
 * or give a value between 0.0 and 1.0
 * to set relative to the parent of the element.
*/
void	ui_element_pos_set(t_ui_element *elem, t_vec4 pos)
{
	if (pos.x < 1.0f && pos.x > 0.0f)
		pos.x = elem->parent_screen_pos->w * pos.x;
	if (pos.y < 1.0f && pos.y > 0.0f)
		pos.y = elem->parent_screen_pos->h * pos.y;
	if (pos.w < 1.0f && pos.w > 0.0f)
		pos.w = elem->parent_screen_pos->w * pos.w;
	if (pos.h < 1.0f && pos.h > 0.0f)
		pos.h = elem->parent_screen_pos->h * pos.h;
	if (elem->pos.w != pos.w || elem->pos.h != pos.h)
		elem->texture_recreate = 1;
	elem->pos = pos;
	elem->screen_pos = ui_element_screen_pos_get(elem);
}

void	ui_element_pos_set2(t_ui_element *elem, t_vec2 pos)
{
	elem->pos.x = pos.x;
	elem->pos.y = pos.y;
}

void	ui_element_color_set(t_ui_element *elem, int state, Uint32 color)
{
	int	j;

	if (state < 0 || state >= UI_STATE_AMOUNT)
		return ;
	if (state == UI_STATE_AMOUNT)
	{
		j = -1;
		while (++j < UI_STATE_AMOUNT)
		{
			if (elem->colors[j] != color)
				elem->texture_recreate = 1;
			elem->colors[j] = color;
		}
		return ;
	}
	if (elem->colors[state] != color)
		elem->texture_recreate = 1;
	elem->colors[state] = color;
}

void	ui_element_image_set_from_path(t_ui_element *elem, int state, char *image_path)
{
	SDL_Surface	*image;

	if (!image_path)
		return ;
	image = IMG_Load(image_path);
	if (!image)
		return ;
	ui_element_image_set(elem, state, image);
	SDL_FreeSurface(image);
}

void	ui_element_image_set(t_ui_element *elem, int state, SDL_Surface *image)
{
	if (state < 0 || state > UI_STATE_AMOUNT)
		return ;
	if (elem->images[state])
		SDL_FreeSurface(elem->images[state]);
	elem->images[state] = ui_surface_new(image->w, image->h);
	SDL_BlitSurface(image, NULL, elem->images[state], NULL);
	elem->use_images = 1;
	elem->texture_recreate = 1;
}

void	dummy_free_er(void *dont, size_t care)
{
	(void)dont;
	(void)care;
}

/*
 * Goes through parent->children and find "elem" if found,
 * it removes it from the list.
*/
void	ui_element_remove_child_from_parent(t_ui_element *elem)
{
	t_list	*curr;
	t_list	**list;

	if (!elem->parent)
	{
		ft_printf("[%s] Element doesnt have a parent.\n", __FUNCTION__);
		return ;
	}
	if (elem->parent_type == UI_TYPE_WINDOW)
	{
		if (((t_ui_window *)elem->parent)->children)
			list = &((t_ui_window *)elem->parent)->children;
		else
		{
			ft_printf("[%s] Parent element didnt have children.\n", __FUNCTION__);
			return ;
		}
	}
	else if (elem->parent_type == UI_TYPE_ELEMENT)
	{
		if (((t_ui_element *)elem->parent)->children)
			list = &((t_ui_element *)elem->parent)->children;
		else
		{
			ft_printf("[%s] Parent element didnt have children.\n", __FUNCTION__);
			return ;
		}
	}
	else
	{
		ft_printf("[%s] Parent of type [%d] %s not supported.\n", __FUNCTION__, elem->parent_type, ui_element_type_to_string(elem->parent_type));
		return ;
	}
	curr = *list;
	while (curr)
	{
		if (curr->content == elem)
			ft_lstdelone_nonfree(list, curr);
		curr = curr->next;
	}
}

/*
 * int	type;		this is the parent type, so we know what to typecast to.
*/
void	ui_element_parent_set(t_ui_element *elem, void *parent, int type)
{
	t_ui_element	*parent_elem;
	t_ui_window		*parent_win;

	if (elem->parent)
		ui_element_remove_child_from_parent(elem);
	if (type == UI_TYPE_WINDOW)
	{
		parent_win = parent;
		elem->parent = parent;
		elem->parent_type = type;
		elem->parent_screen_pos = &parent_win->screen_pos;
		elem->parent_show = &parent_win->show;
		add_to_list(&parent_win->children, elem, UI_TYPE_ELEMENT);
	}
	else if (type == UI_TYPE_ELEMENT)
	{
		parent_elem = parent;
		elem->parent = parent;
		elem->parent_type = type;
		elem->parent_screen_pos = &parent_elem->screen_pos;
		elem->parent_show = &parent_elem->show;
		add_to_list(&parent_elem->children, elem, UI_TYPE_ELEMENT);
	}
	else
		ft_printf("[%s] Element of type %d is not supported.\n", __FUNCTION__, type);
}

void	ui_element_id_set(t_ui_element *elem, char *id)
{
	if (elem->id)
		ft_strdel(&elem->id);
	elem->id = ft_strdup(id);
}

void	ui_element_print(t_ui_element *elem)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\tid : %s\n", elem->id);
	ft_printf("\tpos : ");
	print_vec(elem->pos.v, 4);
	ft_printf("\tscreen_pos : ");
	print_veci(elem->screen_pos.v, 4);
	ft_printf("\tparent_pos : ");
	print_veci(elem->parent_screen_pos->v, 4);
	ft_printf("\tuse_images : %d\n", elem->use_images);
	ft_printf("\tcolors : %#x %#x %#x\n", elem->colors[0], elem->colors[1], elem->colors[2]);
	ft_printf("\tstate : %d\n", elem->state);
	ft_printf("\tlast_state : %d\n", elem->last_state);
	ft_printf("\tparent_type : %d\n", elem->parent_type);
	ft_printf("\telement_type : %d\n", elem->element_type);
	ft_printf("\tshow : %d\n", elem->show);
	ft_printf("\tis_hover : %d\n", elem->is_hover);
	ft_printf("\tis_click : %d\n", elem->is_click);
	ft_printf("\ttexture_recreate : %d\n", elem->texture_recreate);
}

void	ui_element_swap(t_ui_element *one, t_ui_element *two)
{
	t_ui_element	*temp;

	temp = one;
	one = two;
	two = temp;
}

int	ui_element_type_from_string(char *str)
{
	int	k;

	k = -1;
	while (++k < UI_TYPE_AMOUNT)
		if (ft_strequ(str, g_acceptable[k].name))
			return (k);
	return (UI_TYPE_NONE);
}

const char	*ui_element_type_to_string(int type)
{
	if (type < 0)
		return ("none");
	if (type > UI_TYPE_AMOUNT)
		return ("none");
	return (g_acceptable[type].name);
}
