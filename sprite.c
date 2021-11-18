#include "editor.h"

t_sprite	*sprite_new(void)
{
	t_sprite	*sprite;

	sprite = ft_memalloc(sizeof(t_sprite));
	return (sprite);
}

void	sprite_free(t_sprite *sprite)
{
	if (!sprite)
		return ;
	sprite->parent = NULL;
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

/*
 * Removes all traces of this sprite;
*/
void	remove_sprite(t_editor *editor, t_sprite *sprite)
{
	if (!sprite)
		return ;
	remove_from_list(&editor->sprites, sprite);
	if (!sprite->parent)
		ft_printf("[%s] Sprite doesnt have parent... Error!\n", __FUNCTION__);
	else
		remove_from_list(&sprite->parent->sprites, sprite);
	sprite_free(sprite);
	sprite = NULL;
	--editor->sprite_amount;
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

t_sprite	*get_sprite_from_list_at_pos(t_list *list, t_vec2i pos)
{
	t_list		*curr;
	t_sprite	*sprite;

	curr = list;
	while (curr)
	{
		sprite = curr->content;
		if (vec2_in_vec4(pos, sprite->pos))
			return (sprite);
		curr = curr->next;
	}
	return (NULL);
}

t_sprite	*get_sprite_by_id_from_list(t_list *list, int id)
{
	while (list)
	{
		if (((t_sprite *)list->content)->id == id)
			return (list->content);
		list = list->next;	
	}
	return (NULL);
}
