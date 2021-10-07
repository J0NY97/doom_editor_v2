#include "editor.h"

void	wall_new(void)
{
	
}

void	wall_render(t_editor *editor, t_wall *wall, Uint32 color)
{
	t_vec2i	p1;
	t_vec2i	p2;

	p1 = conversion(editor, wall->p1->pos);
	p2 = conversion(editor, wall->p2->pos);
	ui_surface_line_draw(editor->drawing_surface, p1, p2, color);
}
