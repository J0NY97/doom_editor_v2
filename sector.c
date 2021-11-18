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

t_sector	*get_sector_with_id(t_list *sectors, int id)
{
	while (sectors)
	{
		if (((t_sector *)sectors->content)->id == id)
			return (sectors->content);
		sectors = sectors->next;
	}
	return (NULL);
}

t_vec2i	get_sector_center(t_editor *editor, t_sector *sector)
{
	int		i;
	float	x;
	float	y;
	t_list	*wall;
	t_wall	*w;

	x = 0;
	y = 0;
	wall = sector->walls;
	while (wall)
	{
		w = wall->content;
		x += (w->p1->pos.x + w->p2->pos.x);
		y += (w->p1->pos.y + w->p2->pos.y);
		wall = wall->next;
	}
	i = ft_lstlen(sector->walls) * 2;
	if (i < 2)
		return (vec2i(-1, -1));
	return (vec2i(x / i, y / i));
}

t_sector	*get_sector_from_list_around_radius(t_list *list, t_vec2i pos, int allowed_radius)
{
	t_sector	*sec;

	while (list)
	{
		sec = list->content;
		if (vec2_in_vec4(pos,
			vec4i(sec->center.x - allowed_radius, sec->center.y - allowed_radius,
				allowed_radius * 2, allowed_radius * 2)))
			return (sec);
		list = list->next;
	}
	return (NULL);
}

t_sector	*get_sector_by_id_from_list(t_list *list, int id)
{
	while (list)
	{
		if (((t_sector *)list->content)->id == id)
			return (list->content);
		list = list->next;
	}
	return (NULL);
}

/*
 * Removes all traces of this sector;
*/
int	remove_sector(t_editor *editor, t_sector *sector)
{
	if (!sector)
		return (0);
	remove_from_list(&editor->sectors, sector);
	ft_lstdel(&sector->walls, &dummy_free_er);
	free(sector);
	sector = NULL;
	ft_printf("[%s] Sector removed from editor->sectors\n", __FUNCTION__);
	--editor->sector_amount;
	return (1);
}

int	get_next_sector_id(t_list *list)
{
	t_sector	*sector;
	int			total;
	int			should_be_total;
	int			i;

	total = 0;
	i = 0;
	while (list)
	{
		sector = list->content;
		total += sector->id;
		i++;
		list = list->next;
	}
	should_be_total = i * (i + 1) / 2;
	return (should_be_total - total);
}

/*
 * Yoinked from here:
 * http://www.sunshine2k.de/coding/java/Polygon/Convex/polygon.htm
*/
int	check_sector_convexity(t_sector *sector)
{
	t_vec2i	p;
	t_vec2i	v;
	t_vec2i	u;
	int		res;
	t_wall	*w1;
	t_wall	*w2;
	t_list	*curr;

	if (sector->wall_amount < 2)
		return (-1);
	res = 0;
	curr = sector->walls;
	int i = 0;
	while (curr)
	{
		w1 = curr->content;
		w2 = get_connected_wall(sector->walls, w1);
		if (!w2)
		{
			curr = curr->next;
			continue ;
		}
		p = w1->p1->pos; 
		t_vec2i tmp = w1->p2->pos;
		v.x = tmp.x - p.x;
		v.y = tmp.y - p.y;
		u = w2->p2->pos;
		if (i == 0)
			res = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;
		else
		{
			int newres = u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x;
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
		}
		i++;
		curr = curr->next;
	}
	return (1);
}

/*
 * The wall should be sorted, winding doesnt matter;
*/
int	check_point_in_sector(t_sector *sector, t_vec2i p)
{
	t_vec2i	p0;
	t_vec2i	p1;
	int		res;
	int		newres;
	t_wall	*wall;
	t_list	*curr;

	if (sector->wall_amount < 2)
		return (-1);
	res = 0;
	curr = sector->walls;
	int i = 0;
	while (curr)
	{
		wall = curr->content;
		p0 = wall->p1->pos; 
		p1 = wall->p2->pos;
		if (i == 0)
			res = (p.y - p0.y) * (p1.x - p0.x) - (p.x - p0.x) * (p1.y - p0.y);
		else
		{
			newres = (p.y - p0.y) * (p1.x - p0.x) - (p.x - p0.x) * (p1.y - p0.y);
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
		}
		i++;
		curr = curr->next;
	}
	return (1);
}
