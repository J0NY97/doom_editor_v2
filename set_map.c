#include "editor.h"

char	*set_map_info(t_editor *editor)
{
	char	*final;

	final = ft_sprintf("type : info\n %d\t %d\t %d\t\n",
			ft_lstlen(editor->points),
			ft_lstlen(editor->walls),
			ft_lstlen(editor->sectors));
	return (final);
}

char	*set_points(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_point	*point;
	Uint32	id;

	id = 0;
	final = ft_sprintf("type : point\n");
	curr = editor->points;
	while (curr)
	{
		point = curr->content;
		point->id = id++;
		temp = ft_sprintf("%d\t %d\t %d\t\n", point->id, point->pos.x, point->pos.y);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_walls(t_editor *editor)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_wall	*wall;
	Uint32	id;

	id = 0;
	final = ft_sprintf("type : wall\n");
	curr = editor->walls;
	while (curr)
	{
		wall = curr->content;
		wall->id = id++;
		temp = ft_sprintf("%d\t %d\t %d\t\n", wall->id, wall->p1->id, wall->p2->id);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*get_sector_wall_ids(t_sector *sector)
{
	char	*final;
	char	*temp;
	t_list	*curr;
	t_wall	*wall;

	final = NULL;
	curr = sector->walls;
	while (curr)
	{
		wall = curr->content;
		temp = ft_sprintf("%d ", wall->id);
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

char	*set_sectors(t_editor *editor)
{
	char		*final;
	char		*temp;
	t_list		*curr;
	t_sector	*sector;
	t_list		*curr_wall;
	t_wall		*wall;
	Uint32		id;

	id = 0;
	final = ft_sprintf("type : sector\n");
	curr = editor->sectors;
	while (curr)
	{
		sector = curr->content;
		sector->id = id++;
		temp = ft_sprintf("%d\t %s\n", sector->id, get_sector_wall_ids(sector));
		ft_stradd(&final, temp);
		ft_strdel(&temp);
		curr = curr->next;
	}
	return (final);
}

// Should go through all the points, walls, and sectors and reorder their t.ex ids;
// Remove all lonely points, walls, and sectors, so that we dont have useless stuff that could break the game;
void	editor_cleanup(t_editor *editor)
{
	(void)editor;
}

void	set_map(t_editor *editor)
{
	int	fd;

	fd = creat(editor->map_name, O_RDWR);
	if (!fd)
	{
		ft_printf("[%s] Couldn\'t open map file : %s\n", __FUNCTION__, editor->map_name);
		return ;
	}
	editor_cleanup(editor);
	ft_dprintf(fd, "%s\n %s\n %s\n %s\n", set_map_info(editor), set_points(editor), set_walls(editor), set_sectors(editor));
	ft_printf("[%s] Map saved succesfully : [%s]\n", __FUNCTION__, editor->map_name);
	close(fd);
}
