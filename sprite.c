#include "editor.h"

t_sprite	*sprite_new(void)
{
	t_sprite	*sprite;

	sprite = ft_memalloc(sizeof(t_sprite));
	sprite->pos = vec2i(0, 0);
	return (sprite);
}
