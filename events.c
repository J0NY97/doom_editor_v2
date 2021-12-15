/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:03:52 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 15:49:04 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

/*
 * 1. Remove all the points not a part of a sector;
 * 2. Remove all walls with either points NULL;
 * 2.5. Remove all neighbors not connected;
 * 		(if a wall has neighbor but it's corresponding neighbor/sector doesn't
 * 		have same wall as neighbor, or at all);
 * 3. Remove all sectors with no walls;
*/
void	sector_cleanup(t_editor *editor)
{
	remove_all_lonely_walls(editor);
	ft_printf("[%s] Done cleaning walls!\n", __FUNCTION__);
	remove_all_lonely_points(editor);
	ft_printf("[%s] Done cleaning points!\n", __FUNCTION__);
	ft_printf("[%s] Cleanup crew done!\n", __FUNCTION__);
}

/*
 * Search wall using 'point' and remove one of them (rng) and make the other use
 * the other^1 point of the wall being removed. (*1 not this 'point')
*/
void	merge_connected_walls(
	t_editor *editor, t_sector *sector, t_point *point)
{
	t_wall	*w1;
	t_wall	*w2;

	w1 = NULL;
	w2 = NULL;
	get_walls_connected_to_point(sector->walls, point, &w1, &w2);
	if (!w1 || !w2)
	{
		ft_printf("[%s] BIG ERROR : Theres a point with no walls connected to"
			" it.\n", __FUNCTION__);
		return ;
	}
	if (w1->p1 == point && w2->p2 == point)
		w2->p2 = w1->p2;
	else if (w1->p2 == point && w2->p1 == point)
		w2->p1 = w1->p1;
	else
		ft_printf("[%s] BIG ERROR : This should never happen.\n", __FUNCTION__);
	remove_wall(editor, w1);
	remove_point(editor, point);
	return ;
}

void	remove_button_events(t_editor *editor)
{
	int	was_removed;

	was_removed = 0;
	if (editor->remove_button->state == UI_STATE_CLICK)
	{
		if (editor->selected_point && editor->selected_sector)
		{
			merge_connected_walls(editor, editor->selected_sector,
				editor->selected_point);
			editor->selected_point = NULL;
		}
		else if (editor->selected_sector)
			was_removed = remove_sector(editor, editor->selected_sector);
		else if (editor->selected_entity)
			was_removed = remove_entity(editor, editor->selected_entity);
		if (was_removed)
		{
			editor->selected_sector = NULL;
			editor->selected_point = NULL;
			editor->selected_wall = NULL;
			editor->selected_sprite = NULL;
			editor->selected_entity = NULL;
			sector_cleanup(editor);
		}
	}
}

void	sector_hover_info_events(t_editor *editor)
{
	char		*temp;
	t_sector	*sector;

	if (!editor->hovered_sector)
	{
		editor->sector_hover_info_menu->show = 0;
		return ;
	}
	sector = editor->hovered_sector;
	editor->sector_hover_info_menu->show = 1;
	temp = ft_sprintf("Sector Info\n\nID: %d\n%27cFloor-%7cCeiling-\n"
			"Height : %14d %16d\nTexture : %12d %16d\n"
			"Texture Scale : %2.2f %12.2f\n",
			sector->id, ' ', ' ', sector->floor_height, sector->ceiling_height,
			sector->floor_texture, sector->ceiling_texture,
			sector->floor_scale, sector->ceiling_scale);
	ui_label_set_text(editor->sector_hover_info_label, temp);
	ft_strdel(&temp);
	ui_element_pos_set2(editor->sector_hover_info_menu,
		vec2(editor->win_main->mouse_pos.x + 10,
			editor->win_main->mouse_pos.y + 10));
}

void	user_events(t_editor *editor, SDL_Event e)
{
	calculate_hover(editor);
	update_info_label(editor);
	grid_events(editor, e);
	remove_button_events(editor);
	sector_events(editor);
	wall_events(editor);
	point_events(editor);
	texture_menu_events(editor);
	entity_events(editor);
	event_events(editor);
	spawn_events(editor);
	info_menu_events(editor, e);
	sector_hover_info_events(editor);
	save_window_events(editor);
	edit_window_events(editor);
}
