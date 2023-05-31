#ifndef DT_H
#define DT_H

/**
 *sdl_instance struct - Global SDL variables
 *
 *@window: SDL window
 *@renderer: SDL renderer
 *@skybox: pointer to skybox texture
 *@wall_surface: pointer to SDL wall surface
 *@wall_texture: pointer to SDL wall texture
 *@weapon: pointer to SDL weapon texture
 */
typedef struct sdl_instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *skybox;
	SDL_Surface *wall_surface;
	SDL_Texture *wall_texture;
	SDL_Texture *weapon;
} sdl_instance;

/**
 * struct player - Data structure with player information
 * @locale: SDL rectangle representing player position
 * @view_angle: Current viewing angle of the player
 */
typedef struct player
{
	SDL_Rect locale;
	float view_angle;
} player;

/**
 * struct map - Data structure holding map information
 * @arr: 2D array of integer characters
 * @rows: number of rows in the array
 * @columns: number of columns in the array
 */
typedef struct map
{
	char **arr;
	int rows;
	int columns;
} map_t;

/**
 *Data structure for defining a line
 *@p1: starting point of the line (x, y coordinates)
 *@p2: ending point of the line (x, y coordinates)
 */
typedef struct line
{
	SDL_Point p1;
	SDL_Point p2;
} line;

/**
 * struct thread_data - Data structure for storing thread-related information
 *@sdl: sdl_instance structure
 *@map: map_t structure
 *@player: player structure
 *@map_active: pointer to a boolean indicating map status
 *@quit: pointer to an integer representing program state
 */
typedef struct thread_data
{
	sdl_instance *sdl;
	map_t *map;
	struct player *player;
	SDL_bool *map_active;
	int *quit;
} thread_data;

#endif
