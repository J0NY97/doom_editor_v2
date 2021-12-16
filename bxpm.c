/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bxpm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 09:45:20 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/16 11:23:30 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

static void	copy_bxpm_pix_to_surf_pix(t_bxpm *bxpm, SDL_Surface *surface)
{
	int	i;

	i = -1;
	while (++i < bxpm->pix_nb)
		((Uint32 *)surface->pixels)[i] = bxpm->clr[bxpm->pix[i]];
}

SDL_Surface	*load_bxpm_to_surface(char *bxpm_file)
{
	SDL_Surface	*surface;
	t_bxpm		bxpm;

	if (!read_bxpm(&bxpm, bxpm_file))
	{
		ft_printf("[load_bxpm_to_surface] Couldnt open file: %s\n", bxpm_file);
		return (NULL);
	}
	surface = ui_surface_new(bxpm.w, bxpm.h);
	copy_bxpm_pix_to_surf_pix(&bxpm, surface);
	free(bxpm.pix);
	free(bxpm.clr);
	return (surface);
}
