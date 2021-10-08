#include "editor.h"

void	sector_render(t_editor *editor, t_sector *sector, Uint32 color)
{
	draw_walls(editor, sector->walls, color);
}

t_sector	*get_sector_from_list_around_radius(t_list *list, t_vec2i pos, float allowed_radius)
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
		{
			final = sec;
			break ;
		}
		list = list->next;
	}
	return (final);
}
