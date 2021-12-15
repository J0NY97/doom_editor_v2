/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:48:31 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:48:31 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	get_next_sprite_id(t_list *list)
{
	t_sprite	*event;
	int			i;
	char		ids[1024];

	i = -1;
	memset(ids, 0, sizeof(char) * 1024);
	while (list)
	{
		event = list->content;
		ids[event->id] = 1;
		list = list->next;
	}
	while (ids[++i])
		;
	return (i);
}

t_sprite	*get_sprite_from_list_at_pos(t_list *list, t_vec2i pos)
{
	t_list		*curr;
	t_sprite	*sprite;

	curr = list;
	while (curr)
	{
		sprite = curr->content;
		if (vec2_in_vec4(pos, sprite->screen_pos))
			return (sprite);
		curr = curr->next;
	}
	return (NULL);
}

t_sprite	*get_sprite_by_id_from_list(t_list *list, int id)
{
	while (list)
	{
		if (((t_sprite *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}
