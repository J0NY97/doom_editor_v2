#include "libui.h"

/*
 * All the elements share this recipe,
 * when making new element you make it from a recipe,
 * all elements have their own default recipe that will be gotten
 * from "t_new_recipe	ui_element_get_recipe_type(UI_TYPE_...);"
 * then calling "t_ui_element	*ui_element_create_from_recipe([recipe])"
 * and it will call their makers.
 * All of the recipes will be made when they are found in the ui file.
 * When found in one of the elements, an element will be created from recipe,
 * and added to the element children list. The new element will gets parent
 * from the element it was nested inside. All elements can have children.
*/
enum	ui_element_type
{
	UI_ELEMENT_TYPE_BUTTON,
	UI_ELEMENT_TYPE_LABEL,
	UI_ELEMENT_TYPE_WINDOW,
};

typedef struct s_new_window
{
	t_new_element	element; // the element elements will take as parent, all children are in there too;
}					t_new_window;

typedef struct s_new_element
{
	t_vec4i			pos;	// its position;
	t_vec4i			screen_pos; // parent->screen_pos + pos;
	t_new_element	*parent; // self-explanatory;
	t_list			*children; // list of t_new_element;
	ui_element_type	type; // one of the enum in ui_element_type;
}					t_new_element;

typedef struct s_new_recipe
{
	t_vec4i		pos; // position not taking parent into consideration;
	t_list		*children;	// list of t_new_recipe for all the children;
}				t_new_recipe;
