#include "editor.h"

t_sector	*sector_new(void)
{
	t_sector	*sector;

	sector = ft_memalloc(sizeof(t_sector));
	sector->color = random_blue_color();
	sector->floor_height = 0;
	sector->ceiling_height = 20;
	sector->floor_texture = 0;
	sector->ceiling_texture = 0;
	sector->gravity = 20;
	sector->lighting = 0;
	sector->floor_scale = 10.0;
	sector->ceiling_scale = 10.0;
	return (sector);
}

void	sector_render(t_editor *editor, t_sector *sector, Uint32 color)
{
	draw_walls(editor, sector->walls, color);
}

t_sector	*get_sector_from_list_around_radius(t_list *list, t_vec2i pos, int allowed_radius)
{
	t_sector	*final;
	t_sector	*sec;

	final = NULL;
	while (list)
	{
		sec = list->content;
		if (vec2_in_vec4(pos,
			vec4i(sec->center.x - allowed_radius, sec->center.y - allowed_radius,
				sec->center.x + allowed_radius, sec->center.y + allowed_radius)))
			return (sec);
		list = list->next;
	}
	return (final);
}

int	remove_sector(t_editor *editor, t_sector *sector)
{
	if (!sector)
		return (0);
	remove_from_list(&editor->sectors, sector);
	free(sector);
	sector = NULL;
	ft_printf("[%s] Sector removed from editor->sectors\n", __FUNCTION__);
	return (1);
}
