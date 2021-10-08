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
typedef struct	s_entity	t_entity;

/*
 * SDL_Texture		*drawing_texture;		the texture surface will be texturified on and the rendered on screen;
 * SDL_Surface		*drawing_surface;		the surface on where the grid and all the sectors are drawn on;
 * t_vec2i			mouse_pos;				taking into consideration the grid gap_size, every point is default 10 pix x*y;
 * t_vec2i			offset;					when moving the grid this will change, im not sure how i did it last time;
 * TTF_Font			*font;					default font for everything so taht we dont waste fps by closing and opening it;
*/
typedef struct s_editor
{
	t_ui_layout		layout;
	t_ui_window		*win_main;
	t_ui_element	*draw_button;
	t_ui_element	*point_button;
	t_ui_element	*wall_button;
	t_ui_element	*sector_button;

	TTF_Font		*font;

	SDL_Texture		*drawing_texture;
	SDL_Surface		*drawing_surface;
	float			gap_size;
	float			zoom;
	t_vec2i			mouse_pos;
	t_vec2i			offset;

	t_vec2i			first_point;
	t_vec2i			second_point;
	bool			first_point_set;
	bool			second_point_set;

	t_point			*selected_point;
	t_wall			*selected_wall;
	t_sector		*selected_sector;
	t_entity		*selected_entity;

	int				sector_amount;

	t_list			*points;
	t_list			*walls;
	t_list			*sectors;
}					t_editor;

struct s_point
{
	t_vec2i			pos;
};


struct s_wall
{
	Uint32			id;
	t_point			*p1;
	t_point			*p2;
};

struct s_sector
{
	Uint32			id;
	Uint32			color;
	t_vec2i			center;
	t_vec2i			first_point;
	bool			first_point_set;
	int				wall_amount;
	t_list			*walls;
};

struct s_entity	
{
	t_vec2i			pos;
};

// Point
void				point_render(t_editor *editor, t_point *point, Uint32 color);
t_point				*get_point_from_list_around_radius(t_list *points, t_vec2i pos, float allowed_radius);

// Wall
void				wall_render(t_editor *editor, t_wall *wall, Uint32 color);
void				draw_walls(t_editor *editor, t_list *walls, Uint32 color);
t_wall				*get_wall_from_list_around_radius(t_list *list, t_vec2i pos, float allowed_radius);

// Sector
void				sector_render(t_editor *editor, t_sector *sector, Uint32 color);
t_sector			*get_sector_from_list_around_radius(t_list *list, t_vec2i pos, int allowed_radius);

// Help
Uint32				random_blue_color(void);
t_vec2i				conversion(t_editor *editor, t_vec2i v);

#endif
