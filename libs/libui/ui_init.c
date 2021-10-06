#include "libui.h"

/*
 * The init of all the sdl stuff that libui needs.
*/
void	ui_sdl_init(void)
{
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();
}
