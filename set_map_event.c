/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_map_event.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 11:42:40 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 11:42:40 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

char	*create_event_string(t_event *event)
{
	char	*final;
	char	*sector_text;
	int		target_id;

	final = NULL;
	target_id = -1;
	if (event->pointer)
	{
		if (event->pointer_type == TYPE_SECTOR)
			target_id = ((t_sector *)event->pointer)->id;
		else
			target_id = ((t_sprite *)event->pointer)->id;
	}
	if (event->sector)
		sector_text = ft_strdup(event->sector);
	else
		sector_text = ft_strdup("-");
	final = ft_sprintf("%d\t%s\t%s\t%d\t%s\t%d\t%d\t%d\n",
			event->id, g_event_type[event->type],
			g_event_action[event->action], target_id,
			sector_text, event->min, event->max, event->speed);
	ft_strdel(&sector_text);
	return (final);
}

char	*set_event(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_event	*event;
	int		id;

	id = -1;
	final = ft_sprintf("type:event\tTYPE\tACTION\tID\tSEC\tMIN\tMAX\tSPEED\n");
	curr = editor->events;
	while (curr)
	{
		event = curr->content;
		event->id = ++id;
		temp = create_event_string(event);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}
