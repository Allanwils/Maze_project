#include "../headers/maze.h"

/**
 *handle_sdl_error - logs any SDL error
 *Returns: void
 */
void print_sdl_error(void)
{
	printf("SDL Error Occured:%s\n", SDL_GetError());
}

/**
 *init_window - creates an SDL window instance
 *@name: pointer to a string for the window title
 *@sdl: sdl_instance data structure
 *Returns: (0) if the window was not created, otherwise (1)
 */
void create_window(char *name, sdl_instance *sdl)
{
	sdl->window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED,
	SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (!sdl->window)
	{
		print_sdl_error();
		safe_close_sdl(sdl);
		exit(EXIT_FAILURE);
	}
}

/**
 *init_renderer - creates an SDL renderer instance
 *@sdl: sdl_instance data structure
 *Returns: void
 */
void create_renderer(sdl_instance *sdl)
{
	sdl->renderer = SDL_CreateRenderer(sdl->window, -1,
	SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (!sdl->renderer)
	{
		print_sdl_error();
		safe_close_sdl(sdl);
		exit(EXIT_FAILURE);
	}
}

/**
 *cleanup_sdl - releases resources and closes SDL
 *@sdl: sdl_instance data structure
 *Returns: void
 */
void safe_close_sdl(sdl_instance *sdl)
{
	if (sdl->window)
		SDL_DestroyWindow(sdl->window);
	if (sdl->renderer)
		SDL_DestroyRenderer(sdl->renderer);
	if (sdl->skybox)
		SDL_DestroyTexture(sdl->skybox);
	if (sdl->wall_surface)
		SDL_FreeSurface(sdl->wall_surface);
	if (sdl->wall_texture)
		SDL_DestroyTexture(sdl->wall_texture);
	if (sdl->weapon)
		SDL_DestroyTexture(sdl->weapon);
	sdl->window = NULL;
	sdl->renderer = NULL;
	sdl->skybox = NULL;
	sdl->wall_surface = NULL;
	sdl->wall_texture = NULL;
	SDL_Quit();
}
