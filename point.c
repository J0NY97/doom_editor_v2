#include "editor.h"

void	point_render(t_editor *editor, t_point *point, Uint32 color)
{
	ui_surface_circle_draw_filled(editor->drawing_surface, conversion(editor, point->pos), 3, color);
}

t_point	*get_point_with_id(t_list *list, int id)
{
	while (list)
	{
		if (((t_point *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
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

t_point	*get_point_from_wall_at_pos(t_wall *wall, t_vec2i pos)
{
	if (compare_veci(wall->p1->pos.v, pos.v, 2))
		return (wall->p1);
	else if (compare_veci(wall->p2->pos.v, pos.v, 2))
		return (wall->p2);
	return (NULL);
}

t_point	*get_point_from_sector_around_radius(
		t_sector *sector, t_vec2i pos, float allowed_radius)
{
	t_list		*curr;
	t_point		*temp;
	float		x;
	float		y;

	curr = sector->walls;
	while (curr)
	{
		x = -allowed_radius;
		while (x <= allowed_radius)
		{
			y = -allowed_radius;
			while (y <= allowed_radius)
			{
				temp = get_point_from_wall_at_pos(curr->content,
						vec2i(pos.x + (int)x, pos.y + (int)y));
				if (temp)
					return (temp);
				y += 0.5f;
			}
			x += 0.5f;
		}
		curr = curr->next;
	}
	return (temp);
}

/*
 * Removes point from all sectors, walls, and from anything else that are using this point;
*/
int	remove_point(t_editor *editor, t_point *point)
{
	if (!point)
		return (0);
	remove_from_list(&editor->points, point);
	free(point);
	point = NULL;
	ft_printf("[%s] Done removing point.\n", __FUNCTION__);
	return (1);
}

void	remove_from_list(t_list **list, void *pointer)
{
	t_list	*curr;

	curr = *list;
	while (curr)
	{
		if (curr->content == pointer)
			ft_lstdelone_nonfree(list, curr);
		curr = curr->next;
	}
}
