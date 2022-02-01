/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 09:44:05 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/14 09:44:05 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "editor.h"

int	arg_launcher(t_editor *editor, char *arg)
{
	if (ft_strequ(arg, "-l")
		|| ft_strequ(arg, "-launcher"))
	{
		editor->launched_from_launcher = 1;
		return (1);
	}
	return (0);
}

int	arg_map(t_editor *editor, char *arg)
{
	char	**temp;
	int		arr_len;

	if (!ft_strendswith(arg, ".dnd")
		|| !ft_strendswith(arg, ".dnds")
		|| !ft_strendswith(arg, ".dnde"))
	{
		temp = ft_strsplit(arg, '/');
		arr_len = ft_arrlen(temp);
		ft_strremove(temp[arr_len - 1], ".dnde");
		ft_strremove(temp[arr_len - 1], ".dnds");
		ft_strremove(temp[arr_len - 1], ".dnd");
		if (editor->map_name)
			ft_strdel(&editor->map_name);
		editor->map_name = ft_strdup(temp[arr_len - 1]);
		editor->map_full_path = ft_strdup(arg);
		ft_arraydel(temp);
		return (1);
	}
	return (0);
}

/*
 * Returns: 1 if we have given a map_name,
 * 	so it knows if its worth even searching for;
*/
int	args_parser(t_editor *editor, int ac, char **av)
{
	int		index;

	index = 0;
	if (ac < 1)
		return (0);
	while (av[++index])
	{
		if (arg_launcher(editor, av[index]))
			;
		else if (arg_map(editor, av[index]))
			;
	}
	return (0);
}

/*
 * If editor was launched from launcher, we will launch the launcher from the
 * editor;
 */
void	launcher(t_editor *editor)
{
	char *const	arr[] = {ROOT_PATH"play", NULL};

	if (!editor->launched_from_launcher)
		return ;
	execv(arr[0], arr);
}
