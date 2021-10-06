#include "libui.h"

SDL_Surface	*ui_surface_new(int w, int h)
{
	return (SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000));
}

void	ui_surface_fill(SDL_Surface *surface, Uint32 color)
{
	SDL_FillRect(surface, NULL, color);
}

void	ui_surface_fill_rect(SDL_Surface *surface, Uint32 color, t_vec4i rect)
{
	t_rgba	rgba;

	rgba = hex_to_rgba(color);
	SDL_FillRect(surface, &(SDL_Rect){rect.x, rect.y, rect.w, rect.h}, color);
}

void	ui_surface_print(SDL_Surface *surface)
{
	ft_printf("[%s] : \n", __FUNCTION__);
	if (!surface)
	{
		ft_printf("surface given doesn\'t exist.\n");
		return ;
	}
	ft_printf("\tw : %d\n", surface->w);
	ft_printf("\th : %d\n", surface->h);
	ft_printf("\tpitch : %d\n", surface->pitch);
	ft_printf("\tformat : \n");
	ft_printf("\t\tformat : %d %s\n", surface->format->format, SDL_GetPixelFormatName(surface->format->format));
	ft_printf("\t\tBitsPerPixel : %d\n", surface->format->BitsPerPixel);
	ft_printf("\t\tBytesPerPixel : %d\n", surface->format->BytesPerPixel);
	ft_printf("\t\tRmask : %.8x\n", surface->format->Rmask);
	ft_printf("\t\tGmask : %.8x\n", surface->format->Gmask);
	ft_printf("\t\tBmask : %.8x\n", surface->format->Bmask);
	ft_printf("\t\tAmask : %.8x\n", surface->format->Amask);
}

SDL_Surface	*ui_surface_text_new(char *text, char *font_path, int size, Uint32 font_color)
{
	SDL_Surface	*surface;
	TTF_Font	*font;
	t_rgba		rgba;
	SDL_Color	color;

	font = TTF_OpenFont(font_path, size);
	if (!font)
	{
		ft_printf("[%s] Font path doesnt exist.\n", __FUNCTION__);
		return (NULL);
	}
	rgba = hex_to_rgba(font_color);
	color.r = rgba.r;
	color.g = rgba.g;
	color.b = rgba.b;
	color.a = rgba.a;
	surface = TTF_RenderUTF8_Blended(font, text, color);
	TTF_CloseFont(font);
	font = NULL;
	return (surface);
}

SDL_Surface	*ui_surface_image_new(char *image_path)
{
	SDL_Surface	*surface;

	surface = IMG_Load(image_path);
	return (surface);
}

void	ui_surface_pixel_set(SDL_Surface *surface, int x, int y, Uint32 color)
{
	Uint32 *pixels;

	if (!(x >= 0 && x < surface->w && y >= 0 && y < surface->h))
		return ;
	pixels = surface->pixels;
	pixels[y * surface->w + x] = color;
}

Uint32	ui_surface_pixel_get(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels;

	if (!(x >= 0 && x < surface->w && y >= 0 && y < surface->h))
		return (1);
	pixels = surface->pixels;
	return (pixels[y * surface->w + x]);
}

void	ui_surface_line_draw_dot(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer;
	int		increment_val;
	int		end_val;
	int		short_len;
	int		long_len;
	int		dec_inc;
	int		j;
	int		i;
	int		gap;

	y_longer = false;
	short_len = p2.y - p1.y;
	long_len = p2.x - p1.x;
	j = 0;
	i = 0;
	gap = 5;
	y_longer = abs(short_len) > abs(long_len);
	if (y_longer)
		ft_swap(&short_len, &long_len);
	end_val = long_len;
	increment_val = 1;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	dec_inc = 0;
	if (long_len != 0)
		dec_inc = (short_len << 16) / long_len;
	if (y_longer)
	{
		while (i != end_val)
		{
			if (i % 5 != 0) // the 5 is the gap
				ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
		return ;
	}
	while (i != end_val)
	{
		if (i % 5 != 0) // the 5 is the gap
			ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
		j += dec_inc;
		i += increment_val;
	}
}

/*
 * Line algorithm yoinked from:
 * http://elynxsdk.free.fr/ext-docs/Rasterization/Lines/Lines%20algorithms.htm
 * http://www.edepot.com/lined.html
*/
void	ui_surface_line_draw_orig(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer = false;
	int		increment_val;
	int		end_val;
	int		short_len = p2.y - p1.y;
	int		long_len= p2.x - p1.x;
	int		dec_inc;
	int		swap;
	int		j;
	int		i;

	if (abs(short_len) > abs(long_len))
	{
		swap = short_len;
		short_len = long_len;
		long_len = swap;
		y_longer = true;
	}
	end_val = long_len;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	else
		increment_val = 1;
	if (long_len == 0)
		dec_inc = 0;
	else
		dec_inc = (short_len << 16) / long_len;
	j = 0;
	if (y_longer)
	{	
		i = 0;
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
	}
	else
	{
		i = 0;
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
			j += dec_inc;
			i += increment_val;
		}
	}
}

/*
 * Reduced version.
*/
void	ui_surface_line_draw_v2(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer;
	int		increment_val;
	int		end_val;
	int		short_len;
	int		long_len;
	int		dec_inc;
	int		j;
	int		i;

	y_longer = false;
	short_len = p2.y - p1.y;
	long_len = p2.x - p1.x;
	j = 0;
	i = 0;
	y_longer = abs(short_len) > abs(long_len);
	if (y_longer)
		ft_swap(&short_len, &long_len);
	end_val = long_len;
	increment_val = 1;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	dec_inc = 0;
	if (long_len != 0)
		dec_inc = (short_len << 16) / long_len;
	if (y_longer)
	{
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + (j >> 16), p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
		return ;
	}
	while (i != end_val)
	{
		ui_surface_pixel_set(surface, p1.x + i, p1.y + (j >> 16), color);
		j += dec_inc;
		i += increment_val;
	}
}

void	ui_surface_line_draw(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	bool	y_longer;
	int		increment_val;
	int		end_val;
	float		short_len;
	float		long_len;
	float		dec_inc;
	float		j;
	int		i;

	y_longer = false;
	short_len = p2.y - p1.y;
	long_len = p2.x - p1.x;
	j = 0;
	i = 0;
	y_longer = abs(short_len) > abs(long_len);
	if (y_longer)
	{
		float temp = short_len;
		short_len = long_len;
		long_len = temp;
	}
	end_val = long_len;
	increment_val = 1;
	if (long_len < 0)
	{
		increment_val = -1;
		long_len = -long_len;
	}
	dec_inc = 0;
	if (long_len != 0)
		dec_inc = short_len / long_len;
	if (y_longer)
	{
		while (i != end_val)
		{
			ui_surface_pixel_set(surface, p1.x + (int)j, p1.y + i, color);	
			j += dec_inc;
			i += increment_val;
		}
		return ;
	}
	while (i != end_val)
	{
		ui_surface_pixel_set(surface, p1.x + i, p1.y + j, color);
		j += dec_inc;
		i += increment_val;
	}
}

// niklas version
static inline int	sign(float a, float b)
{
	if (a < b)
		return (1);
	return (-1);
}

static inline void	line_calc(t_vec2i v1, t_vec2i v2,
		int *cath_x, int *cath_y)
{
	if (((int)v2.y - (int)v1.y) < 0)
		*cath_x = ((int)v2.y - (int)v1.y) * -1;
	else
		*cath_x = ((int)v2.y - (int)v1.y);
	if (((int)v2.x - (int)v1.x) < 0)
		*cath_y = ((int)v2.x - (int)v1.x) * -1;
	else
		*cath_y = ((int)v2.x - (int)v1.x);
}

void	ui_surface_line_draw_nik(SDL_Surface *surf, t_vec2i v1, t_vec2i v2, Uint32 color)
{
	int			cath_x;
	int			cath_y;
	int			overflow_x;
	int			overflow_y;

	line_calc(v1, v2, &cath_x, &cath_y);
	overflow_y = cath_y - cath_x;
	while ((int)v1.x != (int)v2.x || (int)v1.y != (int)v2.y)
	{
		ui_surface_pixel_set(surf, (int)v1.x, (int)v1.y, color);
		overflow_x = overflow_y * 2;
		if (overflow_x > -(cath_x))
		{
			overflow_y -= cath_x;
			v1.x += sign((int)v1.x, (int)v2.x);
		}
		else if (overflow_x < cath_x)
		{
			overflow_y += cath_y;
			v1.y += sign((int)v1.y, (int)v2.y);
		}
	}
}

/*
 * t_vec2i		p1;			the top left corner of the rect;
 * t_vec2i		p2;			the bot right corner of the rect;
*/
void	ui_surface_rect_draw(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	ui_surface_line_draw(surface, p1, vec2i(p2.x, p1.y), color);
	ui_surface_line_draw(surface, p1, vec2i(p1.x, p2.y), color);
	ui_surface_line_draw(surface, p2, vec2i(p1.x, p2.y), color);
	ui_surface_line_draw(surface, p2, vec2i(p2.x, p1.y), color);
}

void	ui_surface_rect_draw_thicc(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, int thicc, Uint32 color)
{
	int	i;
	int	iter;
	t_vec2i	tl; // top left
	t_vec2i	br; // bot right

	tl.x = ft_min(p1.x, p2.x);
	tl.y = ft_min(p1.y, p2.y);
	br.x = ft_max(p1.x, p2.x);
	br.y = ft_max(p1.y, p2.y);
	iter = ft_min(thicc, dist(tl, br));
	i = -1;
	while (++i < iter)
	{
		ui_surface_line_draw(surface, vec2i(tl.x, tl.y + i), vec2i(br.x, tl.y + i), color); // top left , top right
		ui_surface_line_draw(surface, vec2i(tl.x, br.y - i), vec2i(br.x, br.y - i), color); // bot left , bot right

		ui_surface_line_draw(surface, vec2i(tl.x + i, tl.y), vec2i(tl.x + i, br.y), color); // top left , bot left
		ui_surface_line_draw(surface, vec2i(br.x - i, tl.y), vec2i(br.x - i, br.y), color); // top right , bot right
	}
}

void	ui_surface_rect_draw_filled(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, Uint32 color)
{
	SDL_FillRect(surface, &(SDL_Rect){p1.x, p1.y, p2.x - p1.x, p2.y - p1.y}, color);
}

/*
 * Yoinked from:
 * Filled : https://stackoverflow.com/a/1201227
 * Empty  : https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
*/
void	ui_surface_circle_draw(SDL_Surface *surface, t_vec2i orig, int r, Uint32 color)
{
	int x = 0;
	int y = r;
	int d = 3 - 2 * r;

	ui_surface_pixel_set(surface, orig.x +x, orig.y +y, color);
	ui_surface_pixel_set(surface, orig.x -x, orig.y +y, color);
	ui_surface_pixel_set(surface, orig.x +x, orig.y -y, color);
	ui_surface_pixel_set(surface, orig.x -x, orig.y -y, color);
	ui_surface_pixel_set(surface, orig.x +y, orig.y +x, color);
	ui_surface_pixel_set(surface, orig.x -y, orig.y +x, color);
	ui_surface_pixel_set(surface, orig.x +y, orig.y -x, color);
	ui_surface_pixel_set(surface, orig.x -y, orig.y -x, color);
	while (y >= x)
	{
		x++;
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		ui_surface_pixel_set(surface, orig.x +x, orig.y +y, color);
		ui_surface_pixel_set(surface, orig.x -x, orig.y +y, color);
		ui_surface_pixel_set(surface, orig.x +x, orig.y -y, color);
		ui_surface_pixel_set(surface, orig.x -x, orig.y -y, color);
		ui_surface_pixel_set(surface, orig.x +y, orig.y +x, color);
		ui_surface_pixel_set(surface, orig.x -y, orig.y +x, color);
		ui_surface_pixel_set(surface, orig.x +y, orig.y -x, color);
		ui_surface_pixel_set(surface, orig.x -y, orig.y -x, color);
	}
}

void	ui_surface_circle_draw_filled(SDL_Surface *surface, t_vec2i orig, int r, Uint32 color)
{
	int	x;
	int	y;
	int hh;
	int rx;
	int ph;
	int r_sqr;

	r_sqr = r * r;
	x = -r;	
	while (++x < r)
	{
		hh = sqrt(r_sqr - x * x);
		rx = orig.x + x;
		ph = orig.y + hh;
		y = orig.y - hh;
		while (++y < ph)
			ui_surface_pixel_set(surface, rx, y, color);
	}
}

void	ui_surface_line_draw_thicc(SDL_Surface *surface, t_vec2i p1, t_vec2i p2, int thicc, Uint32 color)
{ 
	float	angle;
	int		i;

	angle = atan2(p2.y - p1.y, p2.x - p1.x);
	i = -1;
	while (++i < thicc)
	{
		t_vec2i v0 = vec2i(p1.x + i * cos(angle + M_PI / 2), p1.y + i * sin(angle + M_PI / 2));
		t_vec2i v3 = vec2i(p2.x + i * cos(angle + M_PI / 2), p2.y + i * sin(angle + M_PI / 2));
		ui_surface_line_draw_nik(surface, v0, v3, color);
    	t_vec2i v1 = vec2i(p1.x + i * cos(angle - M_PI / 2), p1.y + i * sin(angle - M_PI / 2));
	    t_vec2i v2 = vec2i(p2.x + i * cos(angle - M_PI / 2), p2.y + i * sin(angle - M_PI / 2));
		ui_surface_line_draw_nik(surface, v1, v2, color);
	}
}
