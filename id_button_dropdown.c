/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   id_button_dropdown.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 10:26:26 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 10:26:26 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	new_id_button(t_ui_element *parent, char *text, t_ui_recipe *recipe)
{
	t_ui_element	*elem;

	elem = ft_memalloc(sizeof(t_ui_element));
	ui_button_new(parent->win, elem);
	ui_element_set_parent(elem, parent, UI_TYPE_ELEMENT);
	ui_element_edit(elem, recipe);
	ui_button_set_text(elem, text);
}

/*
 * create buttons to 'parent->children' with the texts from 'texts',
 *	all buttons have 'recipe' recipe; Remove all extra buttons in the list;
*/
void	create_buttons_to_list_from_texts_remove_extra(
		t_ui_element *parent, char **texts, t_ui_recipe *recipe)
{
	int				i;
	t_list			*prev_butt;
	t_list			*next_butt;

	i = -1;
	prev_butt = parent->children;
	while (texts && texts[++i])
	{
		if (prev_butt && prev_butt->content)
		{
			ui_button_set_text(prev_butt->content, texts[i]);
			prev_butt = prev_butt->next;
		}
		else
			new_id_button(parent, texts[i], recipe);
	}
	while (prev_butt)
	{
		next_butt = prev_butt->next;
		ui_element_free(prev_butt->content, prev_butt->content_size);
		if (!prev_butt)
			break ;
		prev_butt = next_butt;
	}
}

char	**gen_sector_id_texts(t_list *sectors)
{
	char		**arr;
	char		temp[20];
	t_sector	*sector;

	arr = NULL;
	while (sectors)
	{
		sector = sectors->content;
		arr = ft_arradd(arr, ft_b_itoa(sector->id, temp));
		sectors = sectors->next;
	}
	return (arr);
}

char	**gen_sprite_id_texts(t_list *sprites)
{
	char		**arr;
	char		temp[20];
	t_sprite	*sprite;

	arr = NULL;
	while (sprites)
	{
		sprite = sprites->content;
		arr = ft_arradd(arr, ft_b_itoa(sprite->id, temp));
		sprites = sprites->next;
	}
	return (arr);
}
