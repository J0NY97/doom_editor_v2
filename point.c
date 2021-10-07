#include "editor.h"

void	point_render(t_editor *editor, t_point *point, Uint32 color)
{
	ui_surface_circle_draw_filled(editor->drawing_surface, conversion(editor, point->pos), 3, color);
}

t_point	*get_point_from_list_at_pos(t_list *list, t_vec2i v)
{
	t_list	*curr;
	t_point	*p;

	curr = list;
	while (curr)
	{
		p = curr->content;
		if (compare_veci(p->pos.v, v.v, 2))
			return (curr->content);
		curr = curr->next;
	}
	return (NULL);
}

t_point	*get_point_from_list_around_radius(
		t_list *points, t_vec2i pos, float allowed_radius)
{
	t_point		*temp;
	float		x;
	float		y;

	temp = get_point_from_list_at_pos(points, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_point_from_list_at_pos(points,
					vec2i(pos.x + x, pos.y + y));
			if (temp != NULL)
				break ;
			y += 0.5f;
		}
		if (temp != NULL)
			break ;
		x += 0.5f;
	}
	return (temp);
}

/*
void	point_remove()
{

}
*/
