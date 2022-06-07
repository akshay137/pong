#include "core.h"

#include <SDL2/SDL.h>

result_t uhero_init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return UH_EXTERNERL_LIB_ERROR;
	}
	return UH_SUCCESS;
}

void uhero_shutdown(void)
{
	SDL_Quit();
}