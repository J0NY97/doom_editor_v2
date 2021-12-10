/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmath.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nneronin <nneronin@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 19:03:18 by nneronin          #+#    #+#             */
/*   Updated: 2021/12/10 19:03:20 by nneronin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATH_H
# define MATH_H

int				powi(int n, int pow);
float			distance(int *v1, int *v2, size_t size);
float			get_ratio(t_vec2i orig_wh, t_vec2i new_wh);
float			get_ratio_f(t_vec2 orig_wh, t_vec2 new_wh);

#endif
