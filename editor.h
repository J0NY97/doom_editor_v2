#ifndef EDITOR_H
# define EDITOR_H
# include "fcntl.h"
# include "libui.h"
# include "libft.h"
# include "libpf.h"
# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"

# include "temp.h" // not needed when we are using the real enum.h;

# define MAP_PATH "C:/Users/Jony/source/repos/map_editor_v2/maps/"

typedef struct	s_point		t_point;
typedef struct	s_wall		t_wall;
typedef struct	s_sector	t_sector;
typedef struct	s_entity	t_entity;
typedef struct	s_event		t_event;

typedef struct s_fps
{
	float	curr;
	float	prev;
	int		fps;
	int		count;
}			t_fps;


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

	t_ui_element	*menu_selection;
	t_ui_element	*draw_button;
	t_ui_element	*remove_button;
	t_ui_element	*point_button;
	t_ui_element	*wall_button;
	t_ui_element	*sector_button;
	t_ui_element	*entity_button;
	t_ui_element	*event_button;
	t_ui_element	*save_button;
	t_ui_element	*edit_button;

	t_ui_element	*menu_sector_edit;
	t_ui_element	*close_sector_edit_button;
	t_ui_element	*sector_edit_ok_button;
	t_ui_element	*floor_texture_button;
	t_ui_element	*ceiling_texture_button;
	t_ui_element	*floor_height_input;
	t_ui_element	*ceiling_height_input;
	t_ui_element	*gravity_input;
	t_ui_element	*lighting_input;
	t_ui_element	*floor_texture_scale_input;
	t_ui_element	*ceiling_texture_scale_input;

	t_ui_element	*menu_wall_edit;
	t_ui_element	*close_wall_edit_button;
	t_ui_element	*split_wall_button;
	t_ui_element	*portal_checkbox;
	t_ui_element	*solid_checkbox;
	t_ui_element	*wall_texture_button;
	t_ui_element	*portal_texture_button;
	t_ui_element	*floor_wall_angle_input;
	t_ui_element	*ceiling_wall_angle_input;
	t_ui_element	*wall_texture_scale_input;

	t_ui_element	*texture_menu;
	t_ui_element	*texture_menu_close_button;

	t_ui_element	*entity_edit_menu;
	t_ui_element	*close_entity_edit_button;
	t_ui_element	*entity_yaw_input;
	t_ui_element	*entity_yaw_slider;

	t_ui_element	*mouse_info_label;
	t_ui_element	*sector_info_label;
	t_ui_element	*sub_info_label;

	t_ui_window		*win_save;
	t_ui_element	*story_button;
	t_ui_element	*endless_button;
	t_ui_element	*confirm_save_button;
	t_ui_element	*name_input;

	t_ui_window		*win_edit;

	TTF_Font		*font;

	SDL_Texture		*drawing_texture;
	SDL_Surface		*drawing_surface;
	float			gap_size;
	float			zoom;
	t_vec2i			mouse_pos;
	t_vec2i			offset;

	SDL_Texture		*entity_textures[ENTITY_AMOUNT + 1];

	t_vec2i			first_point;
	t_vec2i			second_point;
	bool			first_point_set;
	bool			second_point_set;

	t_point			*selected_point;
	t_wall			*selected_wall;
	t_sector		*selected_sector;
	t_entity		*selected_entity;
	t_event			*selected_event;

	int				sector_amount;
	int				point_amount;
	int				walls_amount;
	int				entity_amount;
	int				event_amount;

	t_list			*points;
	t_list			*walls;
	t_list			*sectors;
	t_list			*entities;
	t_list			*events;

	char			*map_name;
	char			*map_full_path;
	float			map_scale;
}					t_editor;

// BIG NOTE: the id for all points/walls are made in the saving of the file,
// 			i dont think theres any point trying to reorder stuff before that;
struct s_point
{
	Uint32			id;
	t_vec2i			pos;
};

/*
 * t_vec2i		middle;		a small line coming out from the middle of the wall that will be used to select a wall if there are 2 on top of eachother; the line should also be drawn inwards of the sector;
*/
struct s_wall
{
	Uint32			id;
	t_point			*p1;
	t_point			*p2;

	bool			solid;
	t_sector		*neighbor;
	int				floor_angle;
	int				ceiling_angle;
	float			texture_scale;
};

/*
 * float	floor_scale;		texture scale for floor;
 * float	ceiling_scale;		texture scale for ceiling;
 * int		floor_texture;		texture id for floor;
 * int		ceiling_texture;	texture id for ceiling;
*/
struct s_sector
{
	Uint32			id;
	Uint32			color;
	t_vec2i			center;
	t_vec2i			first_point;
	bool			first_point_set;
	int				wall_amount;
	t_list			*walls;

	int				floor_height;
	int				ceiling_height;
	int				floor_texture;
	int				ceiling_texture;
	int				gravity;
	int 			lighting;
	float			floor_scale;
	float			ceiling_scale;
};

/*
 * int				type;			one from enum e_entity;
 * int				z;				z value of position (dont worry about it);
 * SDL_Texture		*texture;		pointer to one of the editor->entity_textures; (DONT FREE!);
 * int				yaw;			which direction the entity is looking (degrees);
*/
struct s_entity	
{
	int				type;
	t_vec2i			pos;
	int				z;
	SDL_Texture		*texture;
	int				yaw;
};

struct s_event	
{
};

// Point
void				point_render(t_editor *editor, t_point *point, Uint32 color);
t_point				*get_point_from_list_around_radius(t_list *points, t_vec2i pos, float allowed_radius);
t_point				*get_point_from_sector_around_radius(t_sector *sector, t_vec2i pos, float allowed_radius);
int					remove_point(t_editor *editor, t_point *point);

// Wall
t_wall				*wall_new(void);
void				wall_render(t_editor *editor, t_wall *wall, Uint32 color);
void				draw_walls(t_editor *editor, t_list *walls, Uint32 color);
t_wall				*get_wall_from_list_around_radius(t_list *list, t_vec2i pos, float allowed_radius);
t_vec2i				get_wall_middle(t_wall *wall);
int					remove_wall(t_editor *editor, t_wall *wall);

// Sector
t_sector			*sector_new(void);
void				sector_render(t_editor *editor, t_sector *sector, Uint32 color);
t_sector			*get_sector_from_list_around_radius(t_list *list, t_vec2i pos, int allowed_radius);
int					remove_sector(t_editor *editor, t_sector *sector);

// Entity
t_entity			*entity_new(void);
void				entity_render(t_editor *editor, t_entity *entity);
void				entity_yaw_render(t_editor *editor, t_entity *entity);
t_entity			*get_entity_from_list_around_radius(t_list *points, t_vec2i pos, float allowed_radius);

// Get map from args
int					args_parser(t_editor *editor, int ac, char **av);
void				try_reading_map(t_editor *editor);

// Get && Set
void				set_map(t_editor *editor, char *name);

// Help
Uint32				random_blue_color(void);
t_vec2i				conversion(t_editor *editor, t_vec2i v);
void				remove_from_list(t_list **list, void *pointer);

#endif
