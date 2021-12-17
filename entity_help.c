/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:26:45 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/17 15:58:55 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * I know we are starting at 1, not set entity type is 0;
*/
int	get_entity_type(char *text)
{
	int		i;

	i = 0;
	while (++i <= ENTITY_AMOUNT)
	{
		if (ft_strequ(g_entity_data[i - 1].name, text))
			return (i);
	}
	return (0);
}

t_entity	*get_entity_from_list_at_pos(
	t_editor *editor, t_list *list, t_vec2i v)
{
	t_list		*curr;
	t_entity	*p;

	curr = list;
	while (curr)
	{
		p = curr->content;
		if (compare_veci(conversion(editor, p->pos).v, v.v, 2))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_entity	*get_entity_from_list_around_radius(
		t_editor *editor, t_list *points, t_vec2i pos, int allowed_radius)
{
	t_entity	*temp;
	int			x;
	int			y;

	temp = get_entity_from_list_at_pos(editor, points, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_entity_from_list_at_pos(editor, points,
					vec2i(pos.x + x, pos.y + y));
			if (temp != NULL)
				break ;
			y += 1;
		}
		if (temp != NULL)
			break ;
		x += 1;
	}
	return (temp);
}
