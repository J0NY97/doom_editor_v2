/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:31:41 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:31:41 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	get_next_event_id(t_list *list)
{
	t_event		*event;
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

int	get_event_type(char *type_name)
{
	int	ddd;

	ddd = -1;
	while (++ddd < EVENT_TYPE_AMOUNT)
		if (ft_strequ(type_name, g_event_type[ddd]))
			return (ddd);
	return (0);
}

int	get_event_action(char *type_name)
{
	int	ddd;

	ddd = -1;
	while (++ddd < EVENT_ACTION_AMOUNT)
		if (ft_strequ(type_name, g_event_action[ddd]))
			return (ddd);
	return (0);
}
