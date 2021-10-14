#include "editor.h"

t_entity	*entity_new(void)
{
	t_entity	*entity;

	entity = ft_memalloc(sizeof(t_entity));
	return (entity);
}

void	entity_render(SDL_Surface *surface, t_entity *entity)
{
	ui_surface_circle_draw(surface, entity->pos, 5, 0xff0000ff);
}
