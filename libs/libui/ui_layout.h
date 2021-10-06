#include "libui.h"

typedef struct s_ui_layout	t_ui_layout;

struct s_ui_layout
{
	char			*layout_file;
	char			*layout_file_content;
	char			**layout_element_strings;
	char			*style_file;
	char			*style_file_content;
	char			**style_recipe_strings;
	t_list			*windows;
	t_list			*elements;

	t_list			*families;	// t_list of t_ui_family;
	t_list			*recipes; // t_list of t_ui_recipe_v2;
};

typedef struct s_ui_family
{
	char			*parent_id;
	int				parent_type;
	char			**children_strings;
	t_list			*children; // t_list of t_ui_family;
}					t_ui_family;

/*
 * int		value[3];		0 : value, 1 : min, 2 : max;
*/
typedef struct s_ui_recipe
{
	char			*id;
	t_vec4			pos;
	Uint32			bg_colors[UI_STATE_AMOUNT];
	char			*bg_images[UI_STATE_AMOUNT];

	bool			pos_set[VEC4_SIZE];
	bool			bg_colors_set[UI_STATE_AMOUNT];
	bool			bg_images_set[UI_STATE_AMOUNT];

	// Label only
	char			*title;
	Uint32			text_color;
	int				text_align;
	int				text_style;
	char			*font;

	bool			text_color_set;
	bool			text_style_set;

	// Slider only
	int				value[3];
	int				value_set[3];

	// Window only
	char			**flags;
}					t_ui_recipe;

///////////////
// Layout
///////////////
void				ui_layout_event(t_ui_layout *layout, SDL_Event e);
void				ui_layout_render(t_ui_layout *layout);
void				ui_layout_load(t_ui_layout *layout, char *file);
void				layout_apply_style(t_ui_layout *layout);
void				layout_make_recipes(t_ui_layout *layout);
void				layout_split_styles(t_ui_layout *layout);
void				layout_read_style(t_ui_layout *layout);
void				layout_compile_elements(t_ui_layout *layout);
void				layout_make_family_trees(t_ui_layout *layout);
void				layout_split_elements(t_ui_layout *layout);
void				layout_read_file(t_ui_layout *layout, char *file);
