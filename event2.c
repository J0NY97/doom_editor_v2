/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   event2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:32:36 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:32:36 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * Try to activate button with the 'target_id_text',
 * 	if it doesnt exist we update the list,
 * 	and try again.
*/
/*
 * TODO: we have problem here,
 * if you update id drop with more sectors than sprites,
 * and then open sprite ids with less buttons;
*/
void	activate_id_button(t_editor *editor, int type, char *target_id_text)
{
	t_ui_element	*id_button;
	t_ui_recipe		*recipe;
	char			**texts;

	id_button = ui_dropdown_get_button_with_text(
			editor->event_id_dropdown, target_id_text);
	if (!id_button)
	{
		if (type == TYPE_SECTOR)
			texts = gen_sector_id_texts(editor->sectors);
		else
			texts = gen_sprite_id_texts(editor->sprites);
		ft_printf("[%s] Creating new id buttons.\n", __FUNCTION__);
		recipe = ui_layout_get_recipe(&editor->layout, "event_id_button");
		create_buttons_to_list_from_texts_remove_extra(
			ui_dropdown_get_menu_element(editor->event_id_dropdown),
			texts, recipe);
		ft_printf("[%s] Done creating id buttons.\n", __FUNCTION__);
		ft_arraydel(texts);
		id_button = ui_dropdown_get_button_with_text(
				editor->event_id_dropdown, target_id_text);
	}
	ui_dropdown_activate(editor->event_id_dropdown, id_button);
}

void	set_event_ui(t_editor *editor, t_event *event)
{
	char	temp_str[20];

	ui_dropdown_activate(editor->event_type_dropdown,
		ui_dropdown_get_button_with_text(editor->event_type_dropdown,
			(char *)g_event_type[event->type]));
	ui_dropdown_activate(editor->event_action_dropdown,
		ui_dropdown_get_button_with_text(editor->event_action_dropdown,
			(char *)g_event_action[event->action]));
	if (event->pointer)
	{
		if (event->pointer_type == TYPE_SECTOR)
			ft_b_itoa(((t_sector *)event->pointer)->id, temp_str);
		else
			ft_b_itoa(((t_sprite *)event->pointer)->id, temp_str);
		activate_id_button(editor, event->pointer_type, temp_str);
	}
	if (event->sector)
		ui_input_set_text(editor->event_sector_input, event->sector);
	ui_input_set_text(editor->event_min_input, ft_b_itoa(event->min, temp_str));
	ui_input_set_text(editor->event_max_input, ft_b_itoa(event->max, temp_str));
	ui_input_set_text(editor->event_speed_input,
		ft_b_itoa(event->speed, temp_str));
}
