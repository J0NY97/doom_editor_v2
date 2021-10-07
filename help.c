#include "editor.h"

Uint32	random_blue_color(void)
{
	Uint32	color;

	color = rgba_to_hex((t_rgba){
			.a = 255, .r = rand() % 255,
			.g = rand() % 255, .b = 245});
	return (color);
}

t_vec2i	conversion(t_editor *editor, t_vec2i v)
{
	t_vec2i	result;

	result.x = (v.x - editor->offset.x) * (editor->gap_size * editor->zoom);
	result.y = (v.y - editor->offset.y) * (editor->gap_size * editor->zoom);
	return (result);
}


