/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmath.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 09:45:07 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 09:45:07 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMATH_H
# define BMATH_H

int				powi(int n, int pow);
float			distance(int *v1, int *v2, size_t size);
float			get_ratio(t_vec2i orig_wh, t_vec2i new_wh);
float			get_ratio_f(t_vec2 orig_wh, t_vec2 new_wh);

#endif
