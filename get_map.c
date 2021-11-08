#include "editor.h"

void	get_map(t_editor *editor, char *map)
{
	char	*file_content = ft_get_file_content(map);
	ft_putstr(file_content);
	exit(0);
}
