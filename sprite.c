/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:04:29 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/17 13:19:55 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_sprite	*sprite_new(void)
{
	t_sprite	*sprite;

	sprite = ft_memalloc(sizeof(t_sprite));
	return (sprite);
}

void	sprite_free(void *s, size_t size)
{
	t_sprite	*sprite;

	sprite = s;
	if (!sprite)
		return ;
	free(sprite);
	(void)size;
}

t_sprite	*add_sprite(t_editor *editor)
{
	t_sprite	*sprite;

	sprite = sprite_new();
	sprite->pos = vec2(0, 0);
	sprite->texture_id = 0;
	sprite->id = get_next_sprite_id(editor->sprites);
	sprite->scale = 1.0f;
	add_to_list(&editor->sprites, sprite, sizeof(t_sprite));
	++editor->sprite_amount;
	ft_printf("New Sprite Added (%d)\n", editor->sprite_amount);
	return (sprite);
}

/*
 * Removes all traces of this sprite;
*/
void	remove_sprite(t_editor *editor, t_sprite *sprite)
{
	if (!sprite)
		return ;
	remove_from_list(&editor->sprites, sprite);
	if (!sprite->parent)
		ft_printf("[%s] Sprite doesnt have parent... Error!\n", __FUNCTION__);
	else
		remove_from_list(&sprite->parent->sprites, sprite);
	sprite_free(sprite, sizeof(t_sprite));
	sprite = NULL;
	--editor->sprite_amount;
}
