/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_events.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 14:39:16 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 14:39:16 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * NOTE: we dont want to make selected_wall NULL if we dont select a wall;
*/
int	select_wall(t_editor *editor)
{
	t_wall	*wall;

	if (editor->win_main->mouse_down_last_frame != SDL_BUTTON_LEFT
		|| hover_over_open_menus(editor)
		|| !editor->selected_sector)
		return (0);
	wall = get_wall_from_list_around_radius(editor,
			editor->selected_sector->walls, editor->win_main->mouse_pos, 10);
	if (wall)
	{
		editor->selected_wall = wall;
		return (1);
	}
	return (0);
}

int	select_wall_with_arrows(t_editor *editor)
{
	t_list	*wall_tlist;

	if (editor->e.key.type != SDL_KEYDOWN
		|| (editor->e.key.keysym.scancode != SDL_SCANCODE_UP
			&& editor->e.key.keysym.scancode != SDL_SCANCODE_DOWN))
		return (0);
	wall_tlist = get_tlist_with_content(editor->selected_sector->walls,
			editor->selected_wall);
	if (!wall_tlist)
		return (0);
	if (editor->e.key.keysym.scancode == SDL_SCANCODE_UP)
	{
		if (wall_tlist->next)
			editor->selected_wall = wall_tlist->next->content;
		else
			editor->selected_wall = editor->selected_sector->walls->content;
		return (1);
	}
	if (wall_tlist->prev)
		editor->selected_wall = wall_tlist->prev->content;
	else
		editor->selected_wall
			= get_last_tlist(editor->selected_sector->walls)->content;
	return (1);
}

/*
 * NOTE: only move wall if we are NOT hovering over the sprite_edit_menu;
 *
 * 'select_wall' using mouse, 'select_wall_with_arrows' ...
 * 	i think you can figure it out.
*/
void	wall_select_events(t_editor *editor)
{
	if (select_wall(editor) || select_wall_with_arrows(editor))
	{
		editor->active_texture_button = NULL;
		editor->selected_sprite = NULL;
		set_wall_ui(editor, editor->selected_wall);
	}
	else if (editor->selected_wall
		&& editor->win_main->mouse_down == SDL_BUTTON_RIGHT
		&& !(editor->wall_render->show == 1
			&& ui_element_is_hover(editor->sprite_edit_menu)))
		move_wall(editor->selected_wall, editor->move_amount);
	if (editor->close_wall_edit_button->state == UI_STATE_CLICK)
	{
		editor->menu_wall_edit->show = 0;
		editor->selected_wall = NULL;
		editor->selected_sprite = NULL;
	}
	if (editor->selected_wall)
	{
		if (editor->split_wall_button->state == UI_STATE_CLICK)
			split_wall(editor, editor->selected_sector, editor->selected_wall);
		get_wall_ui(editor, editor->selected_wall);
	}
}

void	wall_events(t_editor *editor)
{
	if (editor->sector_button->state == UI_STATE_CLICK
		&& editor->wall_button->state == UI_STATE_CLICK
		&& editor->select_button->state == UI_STATE_CLICK)
	{
		wall_select_events(editor);
		if (editor->selected_wall)
		{
			editor->menu_wall_edit->show = 1;
			editor->sprite_edit_menu->show = 1;
			sprite_events(editor);
			visualize_wall(editor, editor->selected_wall);
			return ;
		}
		editor->selected_sprite = NULL;
		editor->menu_wall_edit->show = 0;
		editor->sprite_edit_menu->show = 0;
	}
	else
	{
		editor->selected_wall = NULL;
		editor->menu_wall_edit->show = 0;
		editor->selected_sprite = NULL;
		editor->sprite_edit_menu->show = 0;
	}
}
