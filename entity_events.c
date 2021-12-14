/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:45:14 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:45:14 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	entity_draw_events(t_editor *editor)
{
	t_entity	*entity;
	t_sector	*sector;

	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
	{
		entity = add_entity(editor);
		entity->pos = editor->mouse_pos;
		sector = point_inside_which_sector(editor->sectors, entity->pos);
		if (sector)
			entity->z = sector->floor_height;
	}
}

/*
 * NOTE: we dont want to overwrite the currently selected entity
 * 		if we dont actually find a new one;
*/
void	select_entity(t_editor *editor)
{
	t_entity	*entity;

	entity = get_entity_from_list_around_radius(
			editor->entities, editor->mouse_pos, 1.0f);
	if (entity)
	{
		editor->selected_entity = entity;
		set_entity_ui(editor, editor->selected_entity);
	}
}

/*
 * NOTE:
 *		If the dropdown menu is open, lets ignore all other inputs;
*/
void	entity_select_events(t_editor *editor)
{
	editor->entity_yaw_input->event = 1;
	editor->entity_yaw_slider->event = 1;
	editor->entity_z_input->event = 1;
	if (ui_dropdown_is_open(editor->entity_dropdown))
	{
		editor->entity_yaw_input->event = 0;
		editor->entity_yaw_slider->event = 0;
		editor->entity_z_input->event = 0;
	}
	if (editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !ui_element_is_hover(editor->entity_edit_menu))
		select_entity(editor);
	else if (editor->selected_entity
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT)
		editor->selected_entity->pos = vec2i_add(
				editor->selected_entity->pos, editor->move_amount);
	if (editor->selected_entity)
		get_entity_ui(editor, editor->selected_entity);
	if (!editor->selected_entity
		|| editor->close_entity_edit_button->state == UI_STATE_CLICK)
	{
		editor->entity_edit_menu->show = 0;
		editor->selected_entity = NULL;
	}
}

void	entity_events(t_editor *editor)
{
	if (editor->entity_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_entity)
			editor->entity_edit_menu->show = 1;
		else
			editor->entity_edit_menu->show = 0;
		if (editor->draw_button->state == UI_STATE_CLICK)
			entity_draw_events(editor);
		else if (editor->select_button->state == UI_STATE_CLICK)
			entity_select_events(editor);
	}
	else
	{
		editor->selected_entity = NULL;
		editor->entity_edit_menu->show = 0;
	}
}
