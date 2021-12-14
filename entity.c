/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:03:44 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/13 15:47:39 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

t_entity	*entity_new(void)
{
	t_entity	*entity;

	entity = ft_memalloc(sizeof(t_entity));
	return (entity);
}

void	entity_free(void *ent, size_t size)
{
	t_entity	*entity;

	entity = ent;
	if (!entity)
		return ;
	free(entity);
	(void)size;
}

t_entity	*add_entity(t_editor *editor)
{
	t_entity	*entity;

	entity = entity_new();
	entity->type = 1;
	++editor->entity_amount;
	add_to_list(&editor->entities, entity, sizeof(t_entity));
	return (entity);
}

int	remove_entity(t_editor *editor, t_entity *entity)
{
	if (!entity)
		return (0);
	remove_from_list(&editor->entities, entity);
	entity_free(entity, sizeof(t_entity));
	--editor->entity_amount;
	ft_printf("[%s] Entity Removed.\n", __FUNCTION__);
	return (1);
}

void	entity_render(t_editor *editor, t_entity *entity)
{
	t_vec2i	www;
	t_vec2i	eee;

	www = conversion(editor, entity->pos);
	eee = vec2i(20, 20);
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	if (editor->entity_textures[entity->type])
	{
		SDL_RenderCopy(editor->win_main->renderer,
			editor->entity_textures[entity->type], NULL,
			&(SDL_Rect){www.x - (eee.x / 2), www.y - (eee.y / 2),
			eee.x, eee.y});
	}
	else
		ft_printf("Entity texture doesnt exist\n");
	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
}
