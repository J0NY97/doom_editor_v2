#include "libui.h"
#include "ui_load.h"

/*
 * Description in function name.
*/
char	**ft_strsplitthatchecksifcharinsidestring(char *str, char c)
{
	char	**arr;
	int		count;
	int		i;
	int		last;
	int		has_been_hash;

	if (!str)
		return (NULL);
	count = 0;
	i = -1;
	last = 0;
	has_been_hash = 0;
	arr = malloc(sizeof(char *) * 1);
	while (str[++i])
	{
		if (str[i] == '"')
			has_been_hash++;
		if (str[i] == c && has_been_hash % 2 == 0)
		{
			arr = realloc(arr, sizeof(char *) * ++count);
			arr[count - 1] = ft_strsub(str, last, i - last);
			last = i + 1;
		}
	}
	if (i != last) // because if the c == last char in str (not variable last, the actual last character)
	{
		arr = realloc(arr, sizeof(char *) * ++count);
		arr[count - 1] = ft_strsub(str, last, i - last);
	}
	arr = realloc(arr, sizeof(char *) * ++count);
	arr[count - 1] = NULL;
	return (arr);
}

#define ALIGN_LEFT "LEFT"
#define ALIGN_RIGHT "RIGHT"
#define ALIGN_TOP "TOP"
#define ALIGN_BOT "BOT"
#define ALIGN_CENTER "CENTER"
#define ALIGN_NONE "NONE"
const char	*text_align_to_str(int text_align)
{
	if (text_align & UI_TEXT_ALIGN_CENTER)
	{
		if (text_align & UI_TEXT_ALIGN_LEFT)
			return (ALIGN_CENTER" "ALIGN_LEFT);
		if (text_align & UI_TEXT_ALIGN_RIGHT)
			return (ALIGN_CENTER" "ALIGN_RIGHT);
		if (text_align & UI_TEXT_ALIGN_TOP)
			return (ALIGN_CENTER" "ALIGN_TOP);
		if (text_align & UI_TEXT_ALIGN_BOT)
			return (ALIGN_CENTER" "ALIGN_BOT);
		else
			return (ALIGN_CENTER);
	}
	else if (text_align & UI_TEXT_ALIGN_LEFT)
	{
		if (text_align & UI_TEXT_ALIGN_TOP)
			return (ALIGN_LEFT" "ALIGN_TOP);
		if (text_align & UI_TEXT_ALIGN_BOT)
			return (ALIGN_LEFT" "ALIGN_BOT);
		else
			return (ALIGN_LEFT);
	}
	else if (text_align & UI_TEXT_ALIGN_RIGHT)
	{
		if (text_align & UI_TEXT_ALIGN_TOP)
			return (ALIGN_RIGHT" "ALIGN_TOP);
		if (text_align & UI_TEXT_ALIGN_BOT)
			return (ALIGN_RIGHT" "ALIGN_BOT);
		else
			return (ALIGN_RIGHT);
	}
	else if (text_align & UI_TEXT_ALIGN_TOP)
		return (ALIGN_TOP);
	else if (text_align & UI_TEXT_ALIGN_BOT)
		return (ALIGN_BOT);
	else if (text_align == 0)
		return (ALIGN_NONE);
	return ("UI_TEXT_ALIGN text not set");
}

int	text_align_getter(char *str)
{
	char	**temp;
	int		result;
	int		i;

	temp = ft_strsplit(str, ' ');
	result = 0;
	i = -1;
	while (temp[++i])
	{
		if (ft_strequ(temp[i], "center"))
			result |= UI_TEXT_ALIGN_CENTER;
		else if (ft_strequ(temp[i], "top"))
			result |= UI_TEXT_ALIGN_TOP;
		else if (ft_strequ(temp[i], "bot"))
			result |= UI_TEXT_ALIGN_BOT;
		else if (ft_strequ(temp[i], "left"))
			result |= UI_TEXT_ALIGN_LEFT;
		else if (ft_strequ(temp[i], "right"))
			result |= UI_TEXT_ALIGN_RIGHT;
		else if (ft_strequ(temp[i], "none"))
			result = UI_TEXT_ALIGN_NONE;
	}
	return (result);
}

/*
 * Not meaning normal C string.
 * but from the ui file '"string"'
*/
char	*trim_string(char *str)
{
	char	*result;
	int		first;
	int		second;

	result = NULL;
	if (!str)
		return (NULL);
	first = ft_strchrlen(str, '"') + 1;
	second = ft_strchrlen(str + first, '"');
	result = ft_strsub(str, first, second);
	return (result);
}
