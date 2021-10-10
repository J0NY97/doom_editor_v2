#include "libui.h"

void	ui_input_new(t_ui_window *win, t_ui_element *elem)
{
	t_ui_input	*input;

	ui_element_new(win, elem);
	input = ft_memalloc(sizeof(t_ui_input));
	input->elem = elem;
	elem->element = input;
	elem->element_type = UI_TYPE_INPUT;
	ui_label_new(win, &input->label);
	ui_label_text_set(&input->label, "Input Text");
	ui_element_parent_set(&input->label, elem, UI_TYPE_ELEMENT);
}

void	ui_input_edit(t_ui_element *elem, t_ui_recipe *recipe)
{
	t_ui_input	*input;

	input = elem->element;
	ui_element_edit(&input->label, recipe);	
	input->input_type = recipe->input_type;
}

void	remove_str_from_n_to_m(char **dest, int n, int m)
{
	char *temp0;
	char *temp1;
	int len;
	
	len = ft_strlen(*dest);
	temp0 = ft_strsub(*dest, 0, n);
	temp1 = ft_strsub(*dest, m, len);
	ft_strdel(dest);
	*dest = ft_strjoin(temp0, temp1);
	ft_strdel(&temp0);
	ft_strdel(&temp1);
}

void	remove_char_after_nth_char(char **dest, int n)
{
	char *temp0;
	char *temp1;
	int len;
	
	len = ft_strlen(*dest);
	temp0 = ft_strsub(*dest, 0, n - 1);
	temp1 = ft_strsub(*dest, n, len);
	ft_strdel(dest);
	*dest = ft_strjoin(temp0, temp1);
	ft_strdel(&temp0);
	ft_strdel(&temp1);
}

void	insert_str_after_nth_char(char **dest, char *src, int n)
{
	char *temp0;
	char *temp1;
	int len;
	
	len = ft_strlen(*dest);
	temp0 = ft_strsub(*dest, 0, n);
	temp1 = ft_strsub(*dest, n, len);
	ft_stradd(&temp0, src);
	ft_stradd(&temp0, temp1);
	ft_strdel(dest);
	*dest = ft_strdup(temp0);
	ft_strdel(&temp0);
	ft_strdel(&temp1);
}

char	*str_remove_all_numbers(char *str)
{
	char	*final;
	char	temp[256];
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			temp[++j] = str[i];
	temp[++j] = '\0';
	final = ft_strdup(temp);
	return (final);
}

char	*str_remove_all_letters(char *str)
{
	char	*final;
	char	temp[256];
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (str[++i])
		if (!ft_isalpha(str[i]))
			temp[++j] = str[i];
	temp[++j] = '\0';
	final = ft_strdup(temp);
	return (final);
}

void	ui_input_event(t_ui_element *elem, SDL_Event e)
{
	t_ui_input	*input;
	t_ui_label	*label;
	int			len;

	input = elem->element;
	label = input->label.element;

	elem->is_hover = ui_element_is_hover(elem);
	if (elem->is_hover == 1 && e.type == SDL_MOUSEBUTTONDOWN)
	{
		if (elem->is_click == 0)
		{
			SDL_StartTextInput();
			input->cursor_on_char_num = ft_strlen(label->text);
			input->cursor_from_char_num = input->cursor_on_char_num;
		}
		elem->is_click = 1;
	}
	if (elem->is_click == 1)
	{
		len = ft_strlen(label->text);
		if (e.type == SDL_TEXTINPUT)
		{
			char	*temp;

			if (input->input_type == 1)
				temp = str_remove_all_letters(e.text.text);
			else if (input->input_type == 2)
				temp = str_remove_all_numbers(e.text.text);
			else
				temp = ft_strdup(e.text.text);
			insert_str_after_nth_char(&label->text, temp, input->cursor_on_char_num);
			input->cursor_on_char_num++;
			ft_strdel(&temp);
		}
		else if (e.type == SDL_KEYDOWN)
		{
			if (KMOD_LCTRL & SDL_GetModState()) // All the modstate thingys should probably come first of all. Because some of them use other keys that are in use without the modstate.
			{
				// This needs to gtfo too.
				int small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
				int big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
				if (e.key.keysym.sym == SDLK_c)
				{
					char *clipboard;

					clipboard = ft_strsub(label->text, small, big - small);
					SDL_SetClipboardText(clipboard);
					ft_strdel(&clipboard);
				}
				else if (e.key.keysym.sym == SDLK_v)
				{
					char *clipboard;
					clipboard = SDL_GetClipboardText();

					// This needs to gtfo
					if (big - small > 0)
					{
						remove_str_from_n_to_m(&label->text, small, big);
						input->cursor_on_char_num = small;
					}
					char	*temp;

					if (input->input_type == 1)
						temp = str_remove_all_letters(clipboard);
					else if (input->input_type == 2)
						temp = str_remove_all_numbers(clipboard);
					else
						temp = ft_strdup(clipboard);
					insert_str_after_nth_char(&label->text, temp, input->cursor_on_char_num);
					input->cursor_on_char_num += ft_strlen(temp);
					SDL_free(clipboard);
					ft_strdel(&temp);
					input->cursor_from_char_num = input->cursor_on_char_num;
				}
				else if (e.key.keysym.sym == SDLK_a)
				{
					input->cursor_from_char_num = 0;
					input->cursor_on_char_num = len;
				}
				else if (e.key.keysym.sym == SDLK_x)
				{
					char *clipboard;

					clipboard = ft_strsub(label->text, small, big - small);
					SDL_SetClipboardText(clipboard);
					ft_strdel(&clipboard);
					// This needs to gtfo
					if (big - small > 0)
					{
						remove_str_from_n_to_m(&label->text, small, big);
						input->cursor_on_char_num = small;
						input->cursor_from_char_num = small;
					}
				}
			}
			else if (e.key.keysym.sym == SDLK_END)
			{
				input->cursor_on_char_num = len;
			}
			else if (e.key.keysym.sym == SDLK_HOME)
			{
				input->cursor_on_char_num = 0;
			}
			else if (e.key.keysym.sym == SDLK_RETURN)
			{
				SDL_StopTextInput();
				elem->is_click = 0;
				input->input_exit = 1;
				return ;
			}
			else if (e.key.keysym.sym == SDLK_LEFT)
			{
				input->cursor_on_char_num--;
			}
			else if (e.key.keysym.sym == SDLK_RIGHT)
			{
				input->cursor_on_char_num++;
			}
			else if (e.key.keysym.sym == SDLK_BACKSPACE || e.key.keysym.sym == SDLK_DELETE)
			{
				int small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
				int big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
				if (big - small == 0)
				{
					if (e.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (len - 1 >= 0 && input->cursor_on_char_num - 1 >= 0)
						{
							remove_str_from_n_to_m(&label->text, input->cursor_on_char_num - 1, input->cursor_on_char_num);
							input->cursor_on_char_num--;
						}
					}
					else
					{
						if (input->cursor_on_char_num < len)
							remove_char_after_nth_char(&label->text, input->cursor_on_char_num + 1);
					}
				}
				else
				{
					remove_str_from_n_to_m(&label->text, small, big);
					input->cursor_on_char_num = small;
				}
			}
			else 
			{
				// This needs to gtfo
				int small = ft_min(input->cursor_on_char_num, input->cursor_from_char_num);
				int big = ft_max(input->cursor_on_char_num, input->cursor_from_char_num);
				if (big - small > 0)
				{
					remove_str_from_n_to_m(&label->text, small, big);
					input->cursor_on_char_num = small;
				}
			}
		} // END of e->type == SDL_KEYDOWN
		if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			if (elem->is_hover != 1)
			{
				elem->is_click = 0;
				input->input_exit = 1;
				//SDL_StopTextInput();
				return ;
			}
			// clicks == 1, move the cursor there
			// clicks == 2, select first word at cursor
			// clicks == 3, select whole text
			e.button.clicks %= 4; // small hackeroni, to cycle all the clicks.
			if (e.button.clicks == 1) // if one click, move the cursor to that point.
			{
				if (elem->is_hover == 1)
				{
					// Dont need y because that doesnt matter. only needed if the text can stack on top of eachother, think about it, hard to explain.
					input->cursor_on_char_num = get_nth_char_of_text_at_x(label->text, elem->win->mouse_pos.x - input->label.screen_pos.x, label->font);
				}
				input->cursor_from_char_num = input->cursor_on_char_num; // IMPORTANT:make this is in hover != 1 
			}
			else if (e.button.clicks == 2) // select word your cursor is at.
			{
				if (elem->is_hover == 1)
				{
					int i = input->cursor_on_char_num - 1;
					while (label->text[i] && !ft_isspace(label->text[i]))
						i--;
					i++;
					input->cursor_from_char_num = ft_clamp(i, 0, len);
					while (label->text[i] && !ft_isspace(label->text[i]))
						i++;
					input->cursor_on_char_num = ft_clamp(i, 0, len);
				}
			}
			else if (e.button.clicks == 3) // if triple click select the whole text.
			{
				if (elem->is_hover == 1)
				{
					input->cursor_from_char_num = 0;
					input->cursor_on_char_num = len;
				}
			}
		}
		else if ((SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) && e.type == SDL_MOUSEMOTION)
		{
			if (elem->is_hover == 1)
			{
				if (e.button.state == SDL_PRESSED)
					input->cursor_from_char_num = input->cursor_on_char_num;
				input->cursor_on_char_num = get_nth_char_of_text_at_x(label->text, elem->win->mouse_pos.x - input->label.screen_pos.x, label->font);
			}
		}
		len = ft_strlen(label->text);
		input->cursor_on_char_num = ft_clamp(input->cursor_on_char_num, 0, len);
		input->cursor_from_char_num = ft_clamp(input->cursor_from_char_num, 0, len);
		// Rethink this trash
		// IMPORTANT: Remember that when you have lshift in the bottom, it will not reset the from_char_num... so whatever you do to this dont change that.!
		int too_long_0 = e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN;
		int too_long_1 = e.type == SDL_TEXTINPUT && (KMOD_LSHIFT & SDL_GetModState()); // this is for capital chars. we dont want the selection to automatically select the just typed capital letter and then you have to unselect it manually before continuing your typing....
		// Reset selection if
		if (too_long_0 && !(KMOD_LCTRL & SDL_GetModState()) && (too_long_1 || !(KMOD_LSHIFT & SDL_GetModState())))
		{
			input->cursor_from_char_num = input->cursor_on_char_num;
		}
		label->texture_recreate = 1;
		len = ft_strlen(label->text);
		input->cursor_on_char_num = ft_clamp(input->cursor_on_char_num, 0, len);
		input->cursor_from_char_num = ft_clamp(input->cursor_from_char_num, 0, len);
	}
}

int	ui_input_render(t_ui_element *elem)
{
	t_ui_input	*input;
	t_ui_label	*label;
	t_vec2i		pos_on;
	t_vec2i		pos_from;

	input = elem->element;
	label = input->label.element;
	if (!ui_element_render(elem))
		return (0);
	input->input_exit = 0;
	if (elem->is_click)
	{
		input->cursor_on_char_x = get_x_of_char_in_text(label->text, input->cursor_on_char_num, label->font) + input->label.pos.x;
		input->cursor_from_char_x = get_x_of_char_in_text(label->text, input->cursor_from_char_num, label->font) + input->label.pos.x;
		pos_on = vec2i(input->cursor_on_char_x + elem->screen_pos.x, elem->screen_pos.y + 2);
		pos_from = vec2i(input->cursor_from_char_x + elem->screen_pos.x, elem->screen_pos.y + 2);
		SDL_SetRenderTarget(elem->win->renderer, elem->win->texture);
		SDL_SetRenderDrawColor(elem->win->renderer, 255, 0, 0, 255);
		SDL_RenderDrawLine(elem->win->renderer, pos_on.x, pos_on.y, pos_on.x, pos_on.y + elem->screen_pos.h - 4);
		SDL_SetRenderDrawColor(elem->win->renderer, 0, 0, 255, 255);
		SDL_RenderDrawLine(elem->win->renderer, pos_from.x, pos_from.y, pos_from.x, pos_from.y + elem->screen_pos.h - 4);

		SDL_SetRenderDrawColor(elem->win->renderer, 0, 0, 255, 255);
		int wmax = ft_max(pos_from.x, pos_on.x);
		int wmin = ft_min(pos_from.x, pos_on.x);
		SDL_RenderFillRect(elem->win->renderer, &(SDL_Rect){wmin, pos_from.y, wmax - wmin, elem->screen_pos.h - 4});
		SDL_SetRenderTarget(elem->win->renderer, NULL);
	}

	ui_label_render(&input->label);
	return (1);
}

void	ui_input_free(void *elem)
{
	(void)elem;
}

/*
 * Getters
*/
t_ui_element	*ui_input_get_label_element(t_ui_element *elem)
{
	return (&((t_ui_input *)elem->element)->label);
}

t_ui_label	*ui_input_get_label(t_ui_element *elem)
{
	return (ui_input_get_label_element(elem)->element);
}

char	*ui_input_text_get(t_ui_element *elem)
{
	t_ui_input	*input;	
	t_ui_label	*label;

	input = elem->element;
	label = input->label.element;
	return (label->text);
}

char	*ui_input_set_text(t_ui_element *elem, char *str)
{
	ui_label_text_set(ui_input_get_label_element(elem), str);
}

int	ui_input_exit(t_ui_element *elem)
{
	return (((t_ui_input *)elem->element)->input_exit);
}
