#include "editor.h"

/*
 * Returns: 1 if we have given a map_name, so it knows if its worth even searching for;
*/
int	args_parser(t_editor *editor, int ac, char **av)
{
	char	**temp;
	int		arr_len;

	if (ac < 1)
		return (0);
	if (av[1])
	{
		if (!ft_strendswith(av[1], ".dnd"))
		{
			if (_WIN32)
				temp = ft_strsplit(av[1], '\\');
			else
				temp = ft_strsplit(av[1], '/');
			arr_len = ft_arrlen(temp);
			ft_strremove(temp[arr_len - 1], ".dnd");
			editor->map_name = temp[arr_len];
			editor->map_full_path = ft_strdup(av[1]);
			ft_arraydel(temp);
			return (1);
		}
	}
	return (0);
}

void	try_reading_map(t_editor *editor)
{
	int	fd;

	fd = open(editor->map_full_path, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("[%s] Couldn't open map : [%s] does it exist [%d].\n", __FUNCTION__, editor->map_full_path, access(editor->map_full_path, F_OK));
		close(fd);
		return ;
	}
	ft_printf("[%s] Map <%s> opened successfully.\n", __FUNCTION__, editor->map_full_path);
	close(fd);
}
