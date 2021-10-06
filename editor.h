#ifndef EDITOR_H
# define EDITOR_H
# include "libui.h"
# include "libft.h"
# include "libpf.h"
# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"

typedef struct	s_point		t_point;
typedef struct	s_wall		t_wall;
typedef struct	s_sector	t_sector;

/*
 * SDL_Texture		*drawing_texture;		the texture surface will be texturified on and the rendered on screen;
 * SDL_Surface		*drawing_surface;		the surface on where the grid and all the sectors are drawn on;
 * t_vec2i			mouse_pos;				taking into consideration the grid gap_size, every point is default 10 pix x*y;
 * t_vec2i			offset;					when moving the grid this will change, im not sure how i did it last time;
*/
typedef struct s_editor
{
	t_ui_layout		layout;
	t_ui_window		*win_main;
	t_ui_element	*draw_button;

	SDL_Texture		*drawing_texture;
	SDL_Surface		*drawing_surface;
	float			gap_size;
	float			zoom;
	t_vec2i			mouse_pos;
	t_vec2i			offset;

	t_list			*points; // remove this;
	t_list			*sectors;
}					t_editor;

struct s_point
{
	t_vec2i			pos;
};

void				point_render(SDL_Surface *surface, t_point *point, float gap_size, float zoom, t_vec2i offset);

struct s_wall
{
	Uint32			id;
	t_point			p1;
	t_point			p2;
};

struct s_sector
{
	Uint32			id;
	t_list			*walls;
};

#endif
