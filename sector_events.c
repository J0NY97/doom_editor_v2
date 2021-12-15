/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sector_events.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:38:09 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:38:09 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * NOTE: We dont want to overwrite the currently selected sector with NULL,
 * 		if we dont find sector on mouseclick pos;
*/
void	select_sector(t_editor *editor)
{
	t_sector	*sector;

	sector = get_sector_around_radius(editor, editor->win_main->mouse_pos, 10);
	if (sector)
	{
		editor->selected_sector = sector;
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
		editor->active_texture_button = NULL;
		set_sector_ui(editor, editor->selected_sector);
	}
}

void	attempt_adding_wall_to_sector(t_editor *editor)
{
	if (editor->first_point_set && editor->second_point_set
		&& compare_veci(editor->first_point.v, editor->second_point.v, 2))
		editor->second_point_set = 0;
	if (editor->first_point_set && editor->second_point_set)
	{
		add_wall_to_sector_at_pos(editor, editor->selected_sector,
			editor->first_point, editor->second_point);
		editor->first_point_set = 0;
		editor->second_point_set = 0;
		if (!editor->selected_sector->first_point_set)
		{
			editor->selected_sector->first_point = editor->first_point;
			editor->selected_sector->first_point_set = 1;
		}
		if (compare_veci(editor->second_point.v,
				editor->selected_sector->first_point.v, 2))
			editor->selected_sector = NULL;
		else
		{
			editor->first_point = editor->second_point;
			editor->first_point_set = 1;
		}
	}
}

/*
 * If we dont have selected_sector, we create one;
 * If we have selected_sector, we add walls to it;
*/
void	sector_draw_events(t_editor *editor)
{
	if (editor->draw_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
	{
		if (!editor->selected_sector)
			editor->selected_sector = add_sector(editor);
		if (editor->selected_sector)
		{
			if (!editor->first_point_set)
			{
				editor->first_point_set = 1;
				editor->first_point = editor->mouse_pos;
			}
			else if (!editor->second_point_set)
			{
				editor->second_point_set = 1;
				editor->second_point = editor->mouse_pos;
			}
			attempt_adding_wall_to_sector(editor);
		}
	}
}

/*
 * NOTE: MOVE SECTOR only if nothing else is selected (point/wall);
*/
void	sector_edit_events(t_editor *editor)
{
	if (editor->select_button->state == UI_STATE_CLICK
		&& editor->win_main->mouse_down_last_frame == SDL_BUTTON_LEFT
		&& !hover_over_open_menus(editor))
		select_sector(editor);
	else if (editor->selected_sector
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !editor->selected_point && !editor->selected_wall)
		move_sector(editor->selected_sector, editor->move_amount);
	if (editor->selected_sector && editor->sector_edit_menu->show)
		get_sector_ui(editor, editor->selected_sector);
	if (editor->sector_edit_ok_button->state == UI_STATE_CLICK)
	{
		editor->selected_wall = NULL;
		editor->selected_point = NULL;
	}
	if (editor->close_sector_edit_button->state == UI_STATE_CLICK)
	{
		editor->sector_edit_menu->show = 0;
		editor->selected_sector = NULL;
		editor->selected_wall = NULL;
		editor->selected_sprite = NULL;
		editor->selected_point = NULL;
	}
}

void	sector_events(t_editor *editor)
{
	if (editor->sector_button->state == UI_STATE_CLICK)
	{
		if (editor->select_button->state == UI_STATE_CLICK)
		{
			sector_edit_events(editor);
			editor->sector_edit_menu->show = 0;
			if (editor->selected_sector)
				editor->sector_edit_menu->show = 1;
		}
		else
		{
			sector_draw_events(editor);
			editor->sector_edit_menu->show = 0;
		}
	}
	else
	{
		editor->selected_sector = NULL;
		editor->selected_wall = NULL;
		editor->selected_sprite = NULL;
		editor->selected_point = NULL;
		editor->sector_edit_menu->show = 0;
		editor->wall_button->state = UI_STATE_DEFAULT;
		editor->point_button->state = UI_STATE_DEFAULT;
	}
}
