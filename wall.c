#include "editor.h"

t_wall	*wall_new(void)
{
	t_wall	*wall;

	wall = ft_memalloc(sizeof(t_wall));
	wall->solid = 1;
	wall->texture_scale = 1.0f;
	return (wall);
}

void	wall_render(t_editor *editor, t_wall *wall, Uint32 color)
{
	t_vec2i	p1;
	t_vec2i	p2;

	p1 = conversion(editor, wall->p1->pos);
	p2 = conversion(editor, wall->p2->pos);
	ui_surface_line_draw(editor->drawing_surface, p1, p2, color);
}

t_list	*get_next_wall_node(t_list *list, t_wall *wall)
{
	t_wall	*dest_wall;

	while (list)
	{
		dest_wall = list->content;
		if (dest_wall->p1 == wall->p2)
			return (list);
		list = list->next;
	}
	return (NULL);
}

void	sort_walls(t_list *list)
{
	t_list	*curr;
	t_wall	*temp;
	t_wall	*w1;
	t_wall	*w2;
	t_point	*point;

	curr = list;
	while (curr && curr->next)
	{
		w1 = curr->content;
		w2 = curr->next->content;
		if (w1->p1 != w2->p2) // if the second point in wall isnt the first point in 2nd wall, they are in wrong order;
		{
			if (w1->p1 == w2->p1) // but if the they are just flipped, just flip them back;
			{
				point = w2->p1;
				w2->p1 = w2->p2;
				w2->p2 = point;
			}
			else // the wall is in the wrong place, so lets move stuff around;
			{
				temp = w2;
				t_list *correct = get_next_wall_node(list, w1);
				curr->next->content = correct->content;
				correct->content = w2;
			}
		}
		curr = curr->next;	
	}
}

float	distance_from_vector_to_wall(t_vec2i p0, t_wall *wall)
{
	t_vec2i		p1;
	t_vec2i		p2;
	float		dist;
	float		up;
	float		down;

	p1 = wall->p1->pos;
	p2 = wall->p2->pos;
	up = (p2.x - p1.x) * (p1.y - p0.y)
		- (p1.x - p0.x) * (p2.y - p1.y);
	down = sqrt(ft_pow(p2.x - p1.x, 2) + ft_pow(p2.y - p1.y, 2));
	dist = up / down;
	return (dist);
}

int	vector_in_rect_of_radius(t_vec2i p, t_vec2i v1, t_vec2i v2, float radius)
{
	float	min;
	float	max;

	min = fmin(v1.x, v2.x);
	max = fmax(v1.x, v2.x);
	if (p.x >= min - radius
		&& p.x <= max + radius)
	{
		min = fmin(v1.y, v2.y);
		max = fmax(v1.y, v2.y);
		if (p.y >= min - radius
			&& p.y <= max + radius)
			return (1);
	}
	return (0);
}

t_wall	*get_wall_with_id(t_list *list, int id)
{
	while (list)
	{
		if (((t_wall *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_wall	*get_wall_from_list_around_radius(
		t_list *list, t_vec2i pos, float allowed_radius)
{
	t_wall		*wall;
	float		dist;

	while (list)
	{
		wall = list->content;
		dist = fabs(distance_from_vector_to_wall(pos, wall));
		if (dist <= allowed_radius)
			if (vector_in_rect_of_radius(pos, wall->p1->pos, wall->p2->pos,
					allowed_radius))
				return (list->content);
		list = list->next;
	}
	return (NULL);
}

t_vec2i	get_wall_middle(t_wall *wall)
{
	t_vec2i	middle;

	middle.x = (wall->p1->pos.x + wall->p2->pos.x) / 2;
	middle.y = (wall->p1->pos.y + wall->p2->pos.y) / 2;
	return (middle);
}

int	remove_wall(t_editor *editor, t_wall *wall)
{
	if (!wall)
		return (0);
	remove_from_list(&editor->walls, wall);
	free(wall);
	wall = NULL;
	ft_printf("[%s] Done removing wall.\n", __FUNCTION__);
	return (1);
}

void	remove_sprite_from_wall(t_sprite *sprite, t_wall *wall)
{
	t_list	*curr;

	curr = wall->sprites;
	while (curr)
	{
		if (curr->content == sprite)
		{
			free(curr->content);
			ft_lstdelone_nonfree(&wall->sprites, curr);
		}
		curr = curr->next;
	}
}

void	remove_wall_list_angles(t_list *list, int which)
{
	t_wall	*wall;

	while (list)
	{
		wall = list->content;
		if (which == 0)
			wall->floor_angle = 0;
		else if (which == 1)
			wall->ceiling_angle = 0;
		list = list->next;
	}
}

t_wall	*get_connected_wall(t_list *list, t_wall *wall)
{
	t_wall	*dest_wall;

	while (list)
	{
		dest_wall = list->content;
		if (dest_wall->p1 == wall->p2)
			return (dest_wall);
		list = list->next;
	}
	return (NULL);
}
