/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalmi <jsalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 09:46:12 by jsalmi            #+#    #+#             */
/*   Updated: 2021/12/18 14:23:42 by jsalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EDITOR_H
# define EDITOR_H
# include "fcntl.h"
# include "sys/stat.h"
# include "libui.h"
# include "liblg.h"
# include "libft.h"
# include "libpf.h"
# include "SDL.h"
# include "SDL_ttf.h"
# include "SDL_image.h"
# include "math.h"
# include "bmath.h"
# include "bxpm.h"
# include "path.h"
# include "resources.h"
# include "enum.h"
# define SKYBOX_AMOUNT 4

typedef struct s_spawn		t_spawn;
typedef struct s_point		t_point;
typedef struct s_wall		t_wall;
typedef struct s_sprite		t_sprite;
typedef struct s_sector		t_sector;
typedef struct s_entity		t_entity;
typedef struct s_event		t_event;

enum e_types_of_stuff
{
	TYPE_NONE,
	TYPE_SECTOR,
	TYPE_SPRITE
};

enum e_map_types
{
	MAP_TYPE_ENDLESS,
	MAP_TYPE_STORY
};

typedef struct s_fps
{
	float	curr;
	float	prev;
	int		fps;
	int		count;
}			t_fps;

struct s_spawn
{
	t_vec2i			pos;
	int				z;
	int				yaw;
	t_sector		*inside_sector;
};

/*
 * t_event	*event;		pointer to the event in editor->events;
*/
typedef struct s_event_elem
{
	t_ui_element	menu;
	t_ui_element	button;
	t_ui_element	id;
	t_ui_element	type;
	t_ui_element	action;
	t_ui_element	target_id;
	t_ui_element	sector;
	t_ui_element	min;
	t_ui_element	max;
	t_ui_element	speed;

	t_event			*event;
}					t_event_elem;

/*
 * t_ui_element		*button;		button of the texture elem; t_ui_button
 * t_ui_element		*image;			image of the texture elem; t_ui_menu
 * int				id;				corresponds to the editor->textures array;
*/
typedef struct s_texture_elem
{
	int				id;
	t_ui_element	button;
	t_ui_element	menu;
	t_ui_element	image;
}					t_texture_elem;

/*
 * t_ui_element	*button;	when clicked, opens the texture_menu;
 * t_ui_element	*image;		menu on which image will be blat;
*/
typedef struct s_texture_comb
{
	t_ui_element	*button;
	t_ui_element	*image;
	int				id;
}					t_texture_comb;

/*
 * SDL_Texture		*drawing_texture;		the texture surface will be
 												texturified on and then rendered
 * SDL_Surface		*drawing_surface;		the surface on where the grid and
 												all the sectors are drawn on;
 * t_vec2i			mouse_pos;				(on grid) taking into consideration
 												the grid gap_size and zoom;
 * t_vec2i			mouse_pos_screen;		'mouse_pos' converted back to screen
 												pos(not same as win->mouse_pos);
 * t_vec2i			offset;					moving the grid changes this;
 *
 * t_list			*texture_elems;			t_texture_elem;
 * t_list			*texture_buttons;		from texture_elem the button, so
 * 												that we can use radio_event on
 * 												it; t_ui_element;
 * t_ui_element		*active_texture_button;	the currently active texture button;
 *
 * t_list			*texture_somethings;		list of t_texture_combs;
 * t_list			*texture_opening_buttons;	t_ui_element->t_ui_button,
 * 												when clicked opens texture_menu;
 * t_ui_element	*active_texture_opening_button; which buttons is currently
 * 													active of the
 * 													texture_opening_buttons;
 *
 * int				map_type;				one of enum e_map_types;
*/
typedef struct s_editor
{
	SDL_Event		e;
	t_fps			fps;

	int				launched_from_launcher;

	t_ui_layout		layout;
	t_ui_window		*win_main;

	t_ui_element	*menu_toolbox_top;
	t_ui_element	*menu_selection;
	t_ui_element	*selection_dropdown_menu;
	t_ui_element	*draw_button;
	t_ui_element	*select_button;
	t_ui_element	*point_button;
	t_ui_element	*wall_button;
	t_ui_element	*sector_button;
	t_ui_element	*entity_button;
	t_ui_element	*event_button;
	t_ui_element	*spawn_button;
	t_ui_element	*remove_button;
	t_ui_element	*edit_button;
	t_ui_element	*save_button;

	t_ui_element	*error_label;

	t_texture_comb	floor_texture_something;
	t_texture_comb	ceiling_texture_something;
	t_ui_element	*sector_edit_menu;
	t_ui_element	*close_sector_edit_button;
	t_ui_element	*sector_edit_ok_button;
	t_ui_element	*sector_skybox_dropdown;
	t_ui_element	*sector_skyboxes[SKYBOX_AMOUNT];
	t_ui_element	*floor_texture_button;
	t_ui_element	*floor_texture_image;
	t_ui_element	*ceiling_texture_button;
	t_ui_element	*ceiling_texture_image;
	t_ui_element	*floor_height_input;
	t_ui_element	*ceiling_height_input;
	t_ui_element	*gravity_input;
	t_ui_element	*lighting_input;
	t_ui_element	*floor_texture_scale_input;
	t_ui_element	*ceiling_texture_scale_input;

	t_texture_comb	wall_texture_something;
	t_texture_comb	portal_texture_something;
	t_ui_element	*menu_wall_edit;
	t_ui_element	*close_wall_edit_button;
	t_ui_element	*split_wall_button;
	t_ui_element	*wall_skybox_dropdown;
	t_ui_element	*wall_skyboxes[SKYBOX_AMOUNT];
	t_ui_element	*portal_checkbox;
	t_ui_element	*solid_checkbox;
	t_ui_element	*wall_texture_button;
	t_ui_element	*wall_texture_image;
	t_ui_element	*portal_texture_button;
	t_ui_element	*portal_texture_image;
	t_ui_element	*floor_wall_angle_input;
	t_ui_element	*ceiling_wall_angle_input;
	t_ui_element	*wall_texture_scale_input;

	t_list			*texture_somethings;
	t_list			*texture_opening_buttons;
	t_ui_element	*active_texture_opening_button;

	t_ui_element	*texture_menu;
	t_ui_element	*texture_menu_label;
	t_ui_element	*texture_menu_close_button;
	t_list			*texture_elems;
	t_list			*texture_buttons;
	t_ui_element	*active_texture_button;
	int				active_texture_button_id;

	t_texture_comb	sprite_texture_something;
	t_ui_element	*sprite_edit_menu;
	t_ui_element	*sprite_add_button;
	t_ui_element	*sprite_confirm_button;
	t_ui_element	*sprite_remove_button;
	t_ui_element	*sprite_scale_input;
	t_ui_element	*sprite_type_dropdown;
	t_ui_element	*sprite_type_static;
	t_ui_element	*sprite_type_loop;
	t_ui_element	*sprite_type_action;
	t_ui_element	*sprite_x_input;
	t_ui_element	*sprite_y_input;
	t_ui_element	*wall_render;

	t_ui_element	*sprite_texture_button;
	t_ui_element	*sprite_texture_image;

	t_ui_element	*entity_edit_menu;
	t_ui_element	*close_entity_edit_button;
	t_ui_element	*entity_image;
	t_ui_element	*entity_dropdown;
	t_ui_element	*entity_yaw_input;
	t_ui_element	*entity_yaw_slider;
	t_ui_element	*entity_z_input;

	t_ui_element	*event_scrollbar;
	t_ui_element	*event_edit_menu;
	t_ui_element	*add_event_button;
	t_ui_element	*remove_event_button;
	t_ui_element	*event_type_dropdown;
	t_ui_element	*event_action_dropdown;
	t_ui_element	*event_id_dropdown;
	t_ui_element	*event_id_menu;
	t_ui_element	*event_sector_input;
	t_ui_element	*event_min_input;
	t_ui_element	*event_max_input;
	t_ui_element	*event_speed_input;
	t_ui_element	*event_menu;
	t_ui_element	*event_type_floor;
	t_ui_element	*event_type_ceiling;
	t_ui_element	*event_type_light;
	t_ui_element	*event_type_store;
	t_ui_element	*event_type_hazard;
	t_ui_element	*event_type_audio;
	t_ui_element	*event_type_spawn;
	t_ui_element	*event_action_click_button;
	t_ui_element	*event_action_shoot_button;
	t_ui_element	*event_action_sector_button;
	t_ui_element	*event_action_null_button;

	t_ui_element	*spawn_edit_menu;
	t_ui_element	*spawn_yaw_input;

	t_ui_element	*mouse_info_label;
	t_ui_element	*sector_info_label;
	t_ui_element	*sub_info_label;
	t_ui_element	*sprite_info_label;
	t_ui_element	*misc_info_label;

	t_ui_element	*info_label;
	Uint32			info_label_timer;
	Uint32			info_label_start_timer;

	t_ui_element	*sector_hover_info_menu;
	t_ui_element	*sector_hover_info_label;
	t_sector		*hovered_sector;

	t_ui_window		*win_save;
	t_ui_element	*story_checkbox;
	t_ui_element	*endless_checkbox;
	t_ui_element	*confirm_save_button;
	t_ui_element	*name_input;

	t_ui_window		*win_edit;
	t_ui_element	*map_scale_input;

	SDL_Texture		*drawing_texture;
	SDL_Surface		*drawing_surface;
	float			gap_size;
	float			zoom;
	t_vec2i			mouse_pos;
	t_vec2i			last_mouse_pos;
	t_vec2i			mouse_pos_screen;
	t_vec2i			move_amount;
	t_vec2			offset;
	SDL_Surface		*grid_surface;
	bool			update_grid;
	int				errors;

	SDL_Surface		*wall_textures[MAP_TEXTURE_AMOUNT];
	SDL_Texture		*entity_textures[ENTITY_AMOUNT + 1];
	SDL_Surface		*entity_texture_surfaces[ENTITY_AMOUNT + 1];

	t_vec2i			first_point;
	t_vec2i			second_point;
	bool			first_point_set;
	bool			second_point_set;

	t_point			*selected_point;
	t_wall			*selected_wall;
	t_sprite		*selected_sprite;
	t_sector		*selected_sector;
	t_entity		*selected_entity;
	t_event			*selected_event;

	int				sector_amount;
	int				point_amount;
	int				wall_amount;
	int				entity_amount;
	int				event_amount;
	int				sprite_amount;

	t_list			*points;
	t_list			*walls;
	t_list			*sectors;
	t_list			*entities;
	t_list			*events;
	t_list			*sprites;

	t_list			*event_elements;
	t_list			*event_element_buttons;
	t_ui_element	*active_event_elem;
	t_event_elem	*selected_event_elem;
	t_list			*event_id_buttons;
	int				event_id_buttons_made;
	int				event_id_buttons_in_use;

	t_spawn			spawn;

	int				map_type;
	char			*map_name;
	char			*map_full_path;
	float			map_scale;
}					t_editor;

// BIG NOTE: the id for all points/walls are made in the saving of the file,
// 			i dont think theres any point trying to reorder stuff before that;
struct s_point
{
	int				id;
	t_vec2i			pos;
};

/*
 * t_vec2i		pos;			the position on the sprite ingame;
 * int			texture_id;		the id of the texture used;
 * int			state;			loop, static, or action;
 * SDL_Surface	*texture;		the pointer of a texture;
 * t_vec4i		screen_pos;		the position of the sprite on the visualized
 * 									wall;
*/
struct s_sprite
{
	t_wall			*parent;
	int				id;
	t_vec2			pos;
	t_vec4i			screen_pos;
	int				texture_id;
	float			scale;
	int				type;

	SDL_Surface		*texture;
};
/*
 * int			neighbor_id;	!ONLY used when getting the map!
 * SDL_Surface	*texture;		pointer of texture youre using for this wall;
 * 									(DONT FREE!)
*/
struct s_wall
{
	int				id;
	t_point			*p1;
	t_point			*p2;

	int				width;
	int				height;

	int				wall_texture;
	int				portal_texture;

	SDL_Surface		*texture;

	int				solid;

	t_sector		*parent_sector;
	t_sector		*neighbor;
	int				neighbor_id;

	int				floor_angle;
	int				ceiling_angle;
	float			texture_scale;

	t_list			*sprites;
	int				sprite_amount;

	int				skybox;
};

/*
 * t_vec2i	center;				the center of sector in grid coordinates;
 * t_vec2i	screen_center;		'center' converted to screen coordinates;
 * float	floor_scale;		texture scale for floor;
 * float	ceiling_scale;		texture scale for ceiling;
 * int		floor_texture;		texture id for floor;
 * int		ceiling_texture;	texture id for ceiling;
*/
struct s_sector
{
	int				id;
	Uint32			color;
	t_vec2i			center;
	t_vec2i			screen_center;
	t_vec2i			first_point;
	bool			first_point_set;
	int				wall_amount;
	t_list			*walls;

	int				floor_height;
	int				ceiling_height;
	int				floor_texture;
	int				ceiling_texture;
	int				gravity;
	int				lighting;
	float			floor_scale;
	float			ceiling_scale;

	int				skybox;
};

/*
 * int				type;			one from enum e_entity;
 * int				z;				z value of position (dont worry about it);
 * SDL_Texture		*texture;		pointer to one of the
 * 										editor->entity_textures; (DONT FREE!);
 * int				yaw;			direction the entity is looking (degrees);
*/
struct s_entity
{
	int				type;
	t_vec2i			pos;
	int				z;
	int				yaw;
	t_sector		*inside_sector;
};

/*
 * int		id;						id of the event;
 * int		type;					e_event_type;
 * int		action;					e_event_action;
 * char		*sector;				no idea;
 * int		min, max, speed;		other info for some of the event types;
 * int		pointer_type;			TYPE_SECTOR/TYPE_SPRITE;
 * void		*pointer;				t_sector/t_sprite depending on 'action';
 * t_event_elem	*elem;				t_event_elem, 't_event' is attached to;
 * 										(DONT FREE! they have their own freer)
*/
struct s_event
{
	int				id;
	int				type;
	int				action;
	char			*sector;
	int				min;
	int				max;
	int				speed;

	int				pointer_type;
	void			*pointer;

	t_event_elem	*elem;
};

// Init
void				editor_init(t_editor *editor);
void				info_init(t_editor *editor);
void				type_dropdown_init(t_editor *editor);
void				load_map_textures(t_editor *editor);
void				edit_window_init(t_editor *editor);
void				save_window_init(t_editor *editor);
void				event_menu_init(t_editor *editor);
void				info_menu_init(t_editor *editor);
void				entity_menu_init(t_editor *editor);
void				texture_menu_init(t_editor *editor);
void				sprite_edit_init(t_editor *editor);
void				wall_edit_init(t_editor *editor);
void				sector_edit_init(t_editor *editor);
void				selection_menu_init(t_editor *editor);
void				draw_init(t_editor *editor);

// Fps
void				update_title_fps(SDL_Window *win, t_fps *fps);
void				fps_func(t_fps *fps);

// Event
void				user_events(t_editor *editor, SDL_Event e);
void				sprite_events(t_editor *editor);
void				sector_events(t_editor *editor);
void				save_window_events(t_editor *editor);
void				edit_window_events(t_editor *editor);
void				grid_events(t_editor *editor, SDL_Event e);
void				wall_events(t_editor *editor);
void				point_events(t_editor *editor);
void				texture_menu_events(t_editor *editor);
void				entity_events(t_editor *editor);
void				spawn_events(t_editor *editor);
void				info_menu_events(t_editor *editor, SDL_Event e);
void				event_removing_events(t_editor *editor);
void				event_adding_events(t_editor *editor);
void				event_events(t_editor *editor);
void				visualize_wall(t_editor *editor, t_wall *wall);
void				calculate_hover(t_editor *editor);
void				update_info_label(t_editor *editor);
void				draw_drawing_wall_len(t_editor *editor);

// Draw
void				user_render(t_editor *editor);
void				draw_grid(t_editor *editor);
void				draw_sectors(t_editor *editor, t_list *sectors);
void				draw_entities(t_editor *editor, t_list *entities);
void				draw_entities_yaw(t_editor *editor, t_list *entities);

// Point
void				point_render(t_editor *editor, t_point *point,
						Uint32 color);
t_point				*get_point_with_id(t_list *list, int id);
t_point				*get_point_from_sector_around_radius(t_sector *sector,
						t_vec2i pos, float allowed_radius);
t_point				*add_point(t_editor *editor);
int					remove_point(t_editor *editor, t_point *point);
void				remove_all_lonely_points(t_editor *editor);
void				point_free(void *p, size_t size);

// Wall
t_wall				*wall_new(void);
void				wall_free(void *w, size_t size);
void				wall_render(t_editor *editor, t_wall *wall, Uint32 color);
void				draw_walls(t_editor *editor, t_list *walls, Uint32 color);
t_wall				*get_wall_with_id(t_list *list, int id);
t_wall				*get_wall_from_list_around_radius(t_editor *editor,
						t_list *list, t_vec2i pos, int allowed_radius);
t_vec2i				get_wall_middle(t_wall *wall);
t_wall				*add_wall(t_editor *editor);
int					remove_wall(t_editor *editor, t_wall *wall);
void				remove_all_lonely_walls(t_editor *editor);
void				remove_wall_list_angles(t_list *list, int which);
t_wall				*get_connected_wall(t_list *list, t_wall *wall);
void				sort_walls(t_list *list);
void				set_wall_ui(t_editor *editor, t_wall *wall);
void				get_wall_ui(t_editor *editor, t_wall *wall);
void				split_wall(t_editor *editor, t_sector *sector,
						t_wall *wall);
void				move_wall(t_wall *wall, t_vec2i move_amount);
bool				can_you_make_portal_of_this_wall(t_list *sector_list,
						t_sector *part_of_sector, t_wall *wall);
t_sprite			*add_sprite_to_wall(t_editor *editor, t_wall *wall);
void				get_walls_connected_to_point(t_list *walls, t_point *p,
						t_wall **w1, t_wall **w2);
t_list				*get_next_wall_node(t_list *list, t_wall *wall);
int					wall_in_any_sector(t_list *sectors, t_wall *wall);
int					wall_in_sector(t_sector *sector, t_wall *wall);
void				remove_all_wall_sprites(t_editor *editor, t_wall *wall);

// Sector
t_sector			*sector_new(void);
void				sector_free(void *sec, size_t size);
t_sector			*add_sector(t_editor *editor);
void				sector_render(t_editor *editor, t_sector *sector,
						Uint32 color);
void				add_wall_to_sector_at_pos(t_editor *editor,
						t_sector *sector, t_vec2i p1, t_vec2i p2);
void				move_sector(t_sector *sector, t_vec2i move_amount);
void				set_sector_ui(t_editor *editor, t_sector *sector);
void				get_sector_ui(t_editor *editor, t_sector *sector);
t_sector			*get_sector_with_id(t_list *sectors, int id);
t_vec2i				get_sector_center(t_sector *sector);
t_sector			*get_sector_around_radius(t_editor *editor, t_vec2i pos,
						int allowed_radius);
int					remove_sector(t_editor *editor, t_sector *sector);
int					get_next_sector_id(t_list *list);
t_sector			*get_sector_by_id_from_list(t_list *list, int id);
int					check_sector_convexity(t_sector *sector);
int					check_point_in_sector(t_sector *sector, t_vec2i p);
t_wall				*get_sector_wall_at_pos(t_sector *sector, t_vec2i p1,
						t_vec2i p2);
t_sector			*point_inside_which_sector(t_list *sectors, t_vec2i point);
void				sector_check_errors(t_editor *editor, t_sector *sector);
int					get_sector_id(t_sector *sector);
void				remove_neighbor_from_walls(t_list *list, t_sector *sector);

// Kind of wall, and kind of not wall
bool				can_you_make_portal_of_this_wall(t_list *sector_list,
						t_sector *part_of_sector, t_wall *wall);
void				remove_wall_from_sector(t_sector *sector, t_wall *wall);

// Entity
t_entity			*entity_new(void);
void				entity_free(void *ent, size_t size);
t_entity			*add_entity(t_editor *editor);
int					remove_entity(t_editor *editor, t_entity *entity);
void				entity_render(t_editor *editor, t_entity *entity);
void				set_entity_ui(t_editor *editor, t_entity *entity);
void				get_entity_ui(t_editor *editor, t_entity *entity);
t_entity			*get_entity_from_list_around_radius(t_editor *editor,
						t_list *points, t_vec2i pos, int allowed_radius);
int					get_entity_type(char *text);
void				entity_check_errors(t_editor *editor, t_entity *entity);

// Event
t_event_elem		*event_element_new(t_ui_element *parent);
void				event_elem_free(void *elem, size_t size);
t_event				*add_event(t_editor *editor);
void				remove_event(t_editor *editor, t_event *event);
void				event_elem_update(t_editor *editor,
						t_event_elem *event_elem);
t_event				*event_new(void);
void				event_free(void *ev, size_t size);
int					get_next_event_id(t_list *list);
void				update_event(t_editor *editor, t_event *event);
void				update_event_elem(t_event_elem *elem);
void				update_id_dropdown(t_editor *editor, int action);
void				set_event_ui(t_editor *editor, t_event *event);
int					get_event_type(char *type_name);
int					get_event_action(char *type_name);
void				realign_event_buttons(t_editor *editor);

// Sprite
t_sprite			*sprite_new(void);
void				sprite_free(void *sprite, size_t size);
int					get_next_sprite_id(t_list *list);
t_sprite			*get_sprite_from_list_at_pos(t_list *list, t_vec2i pos);
t_sprite			*get_sprite_by_id_from_list(t_list *list, int id);
char				**gen_sprite_id_texts(t_list *sprites);
t_sprite			*add_sprite(t_editor *editor);
void				remove_sprite(t_editor *editor, t_sprite *sprite);
void				set_sprite_ui(t_editor *editor, t_sprite *sprite);
void				get_sprite_ui(t_editor *editor, t_sprite *sprite);

// Get map from args
int					args_parser(t_editor *editor, int ac, char **av);
void				launcher(t_editor *editor);

// Set
void				set_map(t_editor *editor, char *name);
char				*set_points(t_editor *editor);
char				*set_walls(t_editor *editor);
char				*set_sprites(t_editor *editor);
char				*set_sectors(t_editor *editor);
char				*set_fandc(t_editor *editor);
char				*set_event(t_editor *editor);

// Get
void				get_map(t_editor *editor, char *map);
void				get_points(t_editor *editor, char **lines, int *i);
void				get_walls(t_editor *editor, char **lines, int *i);
void				get_sprites(t_editor *editor, char **lines);
void				get_entity(t_editor *editor, char **lines, int *i);
void				get_events(t_editor *editor, char **args);
void				get_sectors(t_editor *editor, char **lines, int *i);
void				get_fc(t_editor *editor, char **args);

// Help
t_vec2				align_fill(t_vec2 inside, t_vec4 size, int gap, int i);
Uint32				random_blue_color(void);
t_vec2i				conversion(t_editor *editor, t_vec2i v);
void				remove_from_list(t_list **list, void *pointer);
int					hover_over_open_menus(t_editor *editor);
float				get_aspect(float w, float h);
char				**gen_sector_id_texts(t_list *sectors);
void				create_buttons_to_list_from_texts_remove_extra(
						t_ui_element *parent, char **texts,
						t_ui_recipe *recipe);
void				draw_text(SDL_Surface *surface, char *text, t_vec2i pos,
						Uint32 color);
void				send_info_message(t_editor *editor, char *text);
void				set_elem_parent_and_recipe(t_ui_element *elem, int ui_type,
						t_ui_element *parent, char *recipe_id);
void				draw_arrow(SDL_Surface *surface, t_vec2i start, int len,
						float yaw);
t_list				*get_last_tlist(t_list *list);
t_list				*get_tlist_with_content(t_list *list, void *content);
t_vec2i				get_middle(t_vec2i p1, t_vec2i p2);
float				distance_from_vector_to_line(t_vec2i p0, t_vec2i p1,
						t_vec2i p2);
int					vector_in_rect_of_radius(t_vec2i p, t_vec2i v1, t_vec2i v2,
						float radius);

// Free help
void				surface_array_free(SDL_Surface **surfaces, int amount);
void				texture_array_free(SDL_Texture **textures, int amount);
void				texture_elem_free(void *tex_elem, size_t size);

// Bxpm
SDL_Surface			*load_bxpm_to_surface(char *bxpm_file);

#endif
