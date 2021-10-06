#include "editor.h"

void	point_render(SDL_Surface *surface, t_point *point, float gap_size, float zoom, t_vec2i offset)
{
	ui_surface_circle_draw_filled(surface,
		vec2i((point->pos.x - offset.x) * (gap_size * zoom),
			(point->pos.y - offset.y) * (gap_size * zoom)),
		3, 0xff00ff00);
}

/*
void	point_select()
{

}

void	point_remove()
{

}
*/
