#include "editor.h"

t_entity	*entity_new(void)
{
	t_entity	*entity;

	entity = ft_memalloc(sizeof(t_entity));
	return (entity);
}

void	entity_render(t_editor *editor, t_entity *entity)
{
	t_vec2i	www;
	t_vec2i	eee;

	www = conversion(editor, entity->pos);
	eee = vec2i(20, 20);
//	ui_surface_circle_draw(editor->drawing_surface, conversion(editor, entity->pos), 5, 0xff0000ff);
/*
	SDL_SetRenderTarget(editor->win_main->renderer, editor->drawing_texture);
	SDL_RenderCopy(editor->win_main->renderer, editor->entity_textures[0], NULL, &(SDL_Rect){www.x, www.y, 10, 10});
*/
	SDL_SetRenderTarget(editor->win_main->renderer, editor->win_main->texture);
	SDL_RenderCopy(editor->win_main->renderer, entity->texture, NULL, &(SDL_Rect){www.x - (eee.x / 2), www.y - (eee.y / 2), eee.x, eee.y});

	SDL_SetRenderTarget(editor->win_main->renderer, NULL);
}

void	entity_yaw_render(t_editor *editor, t_entity *entity)
{
	float	angle;
	float	angle2;
	float	angle3;
	t_vec2i	p1;
	t_vec2i	p2;
	t_vec2i	p3;
	t_vec2i	p4;

	p1 = conversion(editor, entity->pos);
	angle = entity->yaw * (M_PI / 180);
	angle2 = (entity->yaw - 45) * (M_PI / 180);
	angle3 = (entity->yaw + 45) * (M_PI / 180);
	p2 = vec2i(cos(angle) * 20.0f + p1.x, sin(angle) * 20.0f + p1.y);
	p3 = vec2i(cos(angle2) * -10.0f + p2.x, sin(angle2) * -10.0f + p2.y);
	p4 = vec2i(cos(angle3) * -10.0f + p2.x, sin(angle3) * -10.0f + p2.y);
	ui_surface_line_draw(editor->drawing_surface, p1, p2, 0xffaaab5d);
	ui_surface_line_draw(editor->drawing_surface, p2, p3, 0xffaaab5d);
	ui_surface_line_draw(editor->drawing_surface, p2, p4, 0xffaaab5d);
}

t_entity	*get_entity_from_list_at_pos(t_list *list, t_vec2i v)
{
	t_list		*curr;
	t_entity	*p;

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

t_entity	*get_entity_from_list_around_radius(t_list *points, t_vec2i pos, float allowed_radius)
{
	t_entity	*temp;
	float		x;
	float		y;

	temp = get_entity_from_list_at_pos(points, pos);
	if (temp)
		return (temp);
	x = -allowed_radius;
	while (x <= allowed_radius)
	{
		y = -allowed_radius;
		while (y <= allowed_radius)
		{
			temp = get_entity_from_list_at_pos(points,
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
