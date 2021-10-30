#include "editor.h"

t_sprite	*sprite_new(void)
{
	t_sprite	*sprite;

	sprite = ft_memalloc(sizeof(t_sprite));
	return (sprite);
}

void	sprite_free(t_sprite *sprite)
{
	free(sprite);
}

void	sprite_print(t_sprite *sprite)
{
	ft_printf("[%s]\n", __FUNCTION__);
	ft_printf("\tpos : ");
	print_veci(sprite->pos.v, 2);
	ft_printf("\ttexture : %d\n", sprite->texture);
	ft_printf("\tscale : %.2f\n", sprite->scale);
	ft_printf("\ttype : %d\n", sprite->type);
}

int	get_next_sprite_id(t_list *list)
{
	t_sprite	*sprite;
	int			total;
	int			should_be_total;
	int			i;

	total = 0;
	i = 0;
	while (list)
	{
		sprite = list->content;
		total += sprite->id;
		i++;
		list = list->next;
	}
	should_be_total = i * (i + 1) / 2;
	return (should_be_total - total);
}
