/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:34:02 by jsalmi            #+#    #+#             */
/*   Updated: 2022/02/27 11:19:00 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Update the selected event with the selected info on the event edit menu;
 * NOTE: this is the first input getter function we made,
 * 		but this is basically 'get_event_ui';
 *
 * char	*active_text; // DONT FREE!
*/
void	update_event2(t_editor *editor, t_event *event)
{
	char	*active_text;
	int		id;

	active_text = ui_dropdown_active_text(editor->event_id_dropdown);
	if (active_text)
	{
		id = ft_atoi(active_text);
		if (event->action == SECTOR)
		{
			event->pointer_type = TYPE_SECTOR;
			event->pointer = get_sector_by_id_from_list(editor->sectors, id);
		}
		else
		{
			event->pointer_type = TYPE_SPRITE;
			event->pointer = get_sprite_by_id_from_list(editor->sprites, id);
		}
	}
}

void	update_event4(t_editor *editor, t_event *event)
{
	char	*active_text;

	active_text = ui_input_get_text(editor->event_sector_input);
	if (active_text)
	{
		if (event->sector)
			ft_strdel(&event->sector);
		if (ft_strequ(active_text, ""))
			event->sector = ft_strdup("-");
		else
			event->sector = ft_strdup(active_text);
	}
}

/*
 * char	*active_text; is pointer to an existing one, DONT FREE!
*/
void	update_event3(t_editor *editor, t_event *event)
{
	char	*active_text;

	active_text = ui_input_get_text(editor->event_min_input);
	if (active_text)
		event->min = ft_atoi(active_text);
	active_text = ui_input_get_text(editor->event_max_input);
	if (active_text)
		event->max = ft_atoi(active_text);
	active_text = ui_input_get_text(editor->event_speed_input);
	if (active_text)
		event->speed = ft_atoi(active_text);
}

void	update_event(t_editor *editor, t_event *event)
{
	int		i;
	char	*active_text;

	active_text = ui_dropdown_active_text(editor->event_type_dropdown);
	i = -1;
	while (++i < EVENT_TYPE_AMOUNT)
		if (ft_strequ(active_text, g_event_type[i]))
			event->type = i;
	active_text = ui_dropdown_active_text(editor->event_action_dropdown);
	i = -1;
	while (++i < EVENT_ACTION_AMOUNT)
		if (ft_strequ(active_text, g_event_action[i]))
			event->action = i;
	update_event2(editor, event);
	update_event3(editor, event);
	update_event4(editor, event);
}

/*
 * From the event in elem, update all the corresponding elems in elem;
*/
void	update_event_elem(t_event_elem *elem)
{
	char	temp[29];

	ui_label_set_text(&elem->id, ft_b_itoa(elem->event->id, temp));
	ui_label_set_text(&elem->type, (char *)g_event_type[elem->event->type]);
	ui_label_set_text(&elem->action,
		(char *)g_event_action[elem->event->action]);
	if (elem->event->pointer)
	{
		if (elem->event->pointer_type == TYPE_SECTOR)
			ui_label_set_text(&elem->target_id,
				ft_b_itoa(((t_sector *)elem->event->pointer)->id, temp));
		else
			ui_label_set_text(&elem->target_id,
				ft_b_itoa(((t_sprite *)elem->event->pointer)->id, temp));
	}
	ui_label_set_text(&elem->sector, elem->event->sector);
	ui_label_set_text(&elem->min, ft_b_itoa(elem->event->min, temp));
	ui_label_set_text(&elem->max, ft_b_itoa(elem->event->max, temp));
	ui_label_set_text(&elem->speed, ft_b_itoa(elem->event->speed, temp));
}
