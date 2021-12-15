/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/15 12:35:13 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/15 12:35:13 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	remove_all_wall_sprites(t_editor *editor, t_wall *wall)
{
	t_list	*curr;
	t_list	*next;

	curr = wall->sprites;
	while (curr)
	{
		next = curr->next;
		remove_sprite(editor, curr->content);
		curr = next;
	}
}

void	move_wall(t_wall *wall, t_vec2i move_amount)
{
	if (!wall)
		return ;
	wall->p1->pos = vec2i_add(wall->p1->pos, move_amount);
	wall->p2->pos = vec2i_add(wall->p2->pos, move_amount);
}

/*
 * if the 2nd point in wall isnt the 1st point in 2nd wall, they in wrong order;
 * 		if the they are just flipped, just flip them back;
 * 		else the wall is in the wrong place, so lets move stuff around;
*/
void	sort_walls(t_list *list)
{
	t_list	*curr;
	t_wall	*w1;
	t_wall	*w2;
	t_list	*correct;

	curr = list;
	while (curr && curr->next)
	{
		w1 = curr->content;
		w2 = curr->next->content;
		if (w1->p1 != w2->p2)
		{
			if (w1->p1 == w2->p1)
				pointer_swap(w2->p1, w2->p2);
			else
			{
				correct = get_next_wall_node(list, w1);
				curr->next->content = correct->content;
				correct->content = w2;
			}
		}
		curr = curr->next;
	}
}

t_sprite	*add_sprite_to_wall(t_editor *editor, t_wall *wall)
{
	t_sprite	*sprite;

	sprite = add_sprite(editor);
	if (editor->active_texture_button_id > -1)
		sprite->texture_id = editor->active_texture_button_id;
	sprite->parent = wall;
	add_to_list(&wall->sprites, sprite, sizeof(t_sprite));
	++wall->sprite_amount;
	return (sprite);
}
