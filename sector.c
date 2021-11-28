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

t_sector	*add_sector(t_editor *editor)
{
	t_sector	*sector;

	sector = sector_new();
	sector->id = get_next_sector_id(editor->sectors);
	++editor->sector_amount;
	add_to_list(&editor->sectors, sector, sizeof(t_sector));
	return (sector);
}

void	sector_render(t_editor *editor, t_sector *sector, Uint32 color)
{
	draw_walls(editor, sector->walls, color);
}

/*
 * Checking if we have anything in p1 and p2 should be checked before this function is called;
*/
void	add_wall_to_sector_at_pos(t_editor *editor, t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_wall	*wall;

	wall = add_wall(editor);
	wall->parent_sector = sector;
	wall->p1 = get_point_from_sector_around_radius(sector, p1, 0.0f);
	if (!wall->p1)
	{
		wall->p1 = add_point(editor);
		wall->p1->pos = p1;
	}
	wall->p2 = get_point_from_sector_around_radius(sector, p2, 0.0f);
	if (!wall->p2)
	{
		wall->p2 = add_point(editor);
		wall->p2->pos = p2;
	}
	++sector->wall_amount;
	add_to_list(&sector->walls, wall, sizeof(t_wall));
}

void	move_sector(t_sector *sector, t_vec2i move_amount)
{
	// NOTE: we only need to move the p1 because we sort the list of walls in the sector rendering;
	t_list	*wall_list;
	t_wall	*wall;

	wall_list = sector->walls;
	while (wall_list)
	{
		wall = wall_list->content;
		wall->p1->pos = vec2i_add(wall->p1->pos, move_amount);
	//	wall->p2->pos = vec2i_add(wall->p2->pos, editor->move_amount); // if we move both points of walls we move every point twice;
		wall_list = wall_list->next;
	}
}

/*
 * From t_sector update the ui values;
*/
void	set_sector_ui(t_editor *editor, t_sector *sector)
{
	char	temp_str[20];

	ft_strnclr(temp_str, 20);
	editor->floor_texture_something->id = &sector->floor_texture;
	editor->ceiling_texture_something->id = &sector->ceiling_texture;
	ui_checkbox_toggle_accordingly(editor->sector_skybox_checkbox, sector->skybox);
	ui_input_set_text(editor->floor_height_input, ft_b_itoa(sector->floor_height, temp_str));
	ui_input_set_text(editor->ceiling_height_input, ft_b_itoa(sector->ceiling_height, temp_str));
	ui_input_set_text(editor->gravity_input, ft_b_itoa(sector->gravity, temp_str));
	ui_input_set_text(editor->lighting_input, ft_b_itoa(sector->lighting, temp_str));
	ui_input_set_text(editor->floor_texture_scale_input, ft_b_ftoa(sector->floor_scale, 2, temp_str));
	ui_input_set_text(editor->ceiling_texture_scale_input, ft_b_ftoa(sector->ceiling_scale, 2, temp_str));
	ui_element_image_set(editor->ceiling_texture_image, UI_STATE_AMOUNT, editor->wall_textures[sector->ceiling_texture]);
	ui_element_image_set(editor->floor_texture_image, UI_STATE_AMOUNT, editor->wall_textures[sector->floor_texture]);
}

/*
 * From ui update the t_sector values;
*/
void	get_sector_ui(t_editor *editor, t_sector *sector)
{
	// TODO: we only need 1 int and 1 float, remove extra when we have this function split up according to norme;
	char	temp_str[20];
	int		f_height;
	int		c_height;
	int 	gravity;
	int 	lighting;
	float	f_scale;
	float	c_scale;

	sector->skybox = editor->sector_skybox_checkbox->is_toggle;

	if (ui_input_exit(editor->floor_height_input))
	{
		f_height = ft_atoi(ui_input_get_text(editor->floor_height_input));
		f_height = ft_min(f_height, sector->ceiling_height);
		ft_b_itoa(f_height, temp_str);
		ui_input_set_text(editor->floor_height_input, temp_str);
		sector->floor_height = f_height;
	}
	if (ui_input_exit(editor->ceiling_height_input))
	{
		c_height = ft_atoi(ui_input_get_text(editor->ceiling_height_input));
		c_height = ft_max(sector->floor_height, c_height);
		ft_b_itoa(c_height, temp_str);
		ui_input_set_text(editor->ceiling_height_input, temp_str);
		sector->ceiling_height = c_height;
	}
	if (ui_input_exit(editor->gravity_input))
	{
		gravity = ft_atoi(ui_input_get_text(editor->gravity_input));
		gravity = ft_clamp(gravity, 0, 100);
		ft_b_itoa(gravity, temp_str);
		ui_input_set_text(editor->gravity_input, temp_str);
		sector->gravity = gravity;
	}
	if (ui_input_exit(editor->lighting_input))
	{
		lighting = ft_atoi(ui_input_get_text(editor->lighting_input));
		lighting = ft_clamp(lighting, 0, 100);
		ft_b_itoa(lighting, temp_str);
		ui_input_set_text(editor->lighting_input, temp_str);
		sector->lighting = lighting;
	}

	if (ui_input_exit(editor->floor_texture_scale_input))
	{
		f_scale = ft_atof(ui_input_get_text(editor->floor_texture_scale_input));
		f_scale = ft_fclamp(f_scale, 0.1f, 100.0f);
		ft_b_ftoa(f_scale, 2, temp_str);
		ui_input_set_text(editor->floor_texture_scale_input, temp_str);
		sector->floor_scale = f_scale;
	}
	if (ui_input_exit(editor->ceiling_texture_scale_input))
	{
		c_scale = ft_atof(ui_input_get_text(editor->ceiling_texture_scale_input));
		c_scale = ft_fclamp(c_scale, 0.1f, 100.0f);
		ft_b_ftoa(c_scale, 2, temp_str);
		ui_input_set_text(editor->ceiling_texture_scale_input, temp_str);
		sector->ceiling_scale = c_scale;
	}
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

t_vec2i	get_sector_center(t_sector *sector)
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

t_wall	*get_sector_wall_at_pos(t_sector *sector, t_vec2i p1, t_vec2i p2)
{
	t_list	*curr;
	t_wall	*curr_wall;

	curr = sector->walls;
	while (curr)
	{
		curr_wall = curr->content;
		if ((compare_veci(curr_wall->p1->pos.v, p1.v, 2)
			&& compare_veci(curr_wall->p2->pos.v, p2.v, 2))
			|| (compare_veci(curr_wall->p1->pos.v, p2.v, 2)
			&& compare_veci(curr_wall->p2->pos.v, p1.v, 2)))
			return (curr_wall);
		curr = curr->next;
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
 * https://github.com/Epicurius
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
		p = w1->p2->pos; 
		v = w1->p1->pos; // This is the one we're comparing;
		u = w2->p2->pos;
		if (i == 0)
			res = (u.x - p.x) * (v.y - p.y) - (u.y - p.y) * (v.x - p.x);
		else
		{
			int newres = (u.x - p.x) * (v.y - p.y) - (u.y - p.y) * (v.x - p.x);
			if ((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
			res = newres;
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
			if (newres == 0 || res == 0 || (newres > 0 && res < 0) || (newres < 0 && res > 0))
				return (0);
			res = newres;
		}
		i++;
		curr = curr->next;
	}
	return (1);
}

void	entity_inside_which_sector(t_list *sectors, t_entity *entity)
{
	t_list	*curr;

	curr = sectors;
	entity->inside_sector = NULL;
	while (curr)
	{
		if (check_point_in_sector(curr->content, entity->pos) == 1)
		{
			entity->inside_sector = curr->content;
			break ;
		}
		curr = curr->next;
	}
}

void	sector_check_errors(t_editor *editor, t_sector *sector)
{
	if (!check_sector_convexity(sector))
	{
		draw_text(editor->drawing_surface, "Not Convex!",
			editor->font, sector->screen_center, 0xffff0000); 
		editor->errors += 1;
	}
	if (sector->ceiling_height - sector->floor_height < 0)
	{
		draw_text(editor->drawing_surface,
			"Floor & Ceiling Height Doesn\'t Make Sense!",
			editor->font, sector->screen_center, 0xffffff00); 
		editor->errors += 1;
	}
}
