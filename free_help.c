/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 14:41:44 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/11 14:41:56 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

void	surface_array_free(SDL_Surface **surfaces, int amount)
{
	while (--amount >= 0)
		SDL_FreeSurface(surfaces[amount]);
}

void	texture_array_free(SDL_Texture **textures, int amount)
{
	while (--amount >= 0)
		SDL_DestroyTexture(textures[amount]);
}

/*
 * layout_free, has already freed all the elements in this struct,
 *	so we only need to free the 'tex_elem';
*/
void	texture_elem_free(void *tex_elem, size_t size)
{
	t_texture_elem	*elem;

	elem = tex_elem;
	if (!elem)
		return ;
	free(elem);
	(void)size;
}
