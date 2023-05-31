#include "../headers/maze.h"

/**
 * Draw_untextured_sky - Renders a plain ceiling on the screen
 * @sdl: sdl_instance data structure instance
 * Return: void
 */
void untextured_sky(sdl_instance *sdl)
{
	SDL_Rect sky_dome = {0, 0, SCREEN_WIDTH, (SCREEN_HEIGHT >> 1)};

	REND_COLOR(sdl->renderer, 25, 181, 254, 255);
	SDL_RenderFillRect(sdl->renderer, &sky_dome);
}

/**
 * Draw_untextured_floor - Renders a plain floor on the screen
 * @sdl: instance of sdl data structure
 * Return: void
 */
void untextured_floor(sdl_instance *sdl)
{
	double offset_y = (SCREEN_HEIGHT >> 1);
	SDL_Rect sky_dome = {0, offset_y, SCREEN_WIDTH, (SCREEN_HEIGHT >> 1)};

	REND_COLOR(sdl->renderer, 52, 140, 49, 255);
	SDL_RenderFillRect(sdl->renderer, &sky_dome);
}

/**
 * check_player_collision - identifies player collision with walls
 * @player: pointer to player data structure keeping player information
 * @map: pointer to 2D grid
 * Return: void
 */
void player_collision_detection(player *player, map_t *map)
{
	SDL_Rect wall = {0, 0, GRID_SIZE, GRID_SIZE};
	int i, j;
	int border = 5;
	int dimensions = player->locale.w + border;
	SDL_Rect bounding_box = {player->locale.x - (border / 2),
		player->locale.y - (border / 2),
		dimensions, dimensions};

	for (i = 0; i < map->rows; i++)
	{
		for (j = 0; j < map->columns; j++)
		{
			if (map->arr[i][j] == '0')
				continue;
			wall.x = (j << 4) + MAP_MARGIN;
			wall.y = (i << 4) + MAP_MARGIN;
			if (SDL_HasIntersection(&bounding_box, &wall))
				slide_on_wall(player);
		}
	}
}

/**
 * check_intersect_orientation - Determines line intersection with
 * rectangle sides
 * @wall: SDL_Rect data structure representing  rectangle to check for
 * side intersection
 * @pointA: Pointer to the starting point of the line (ray)
 * @pointB: Pointer to the end point of the line
 *Returns: (1) if intersection occurred on  top or bottom of  rectangle,
 *otherwise (2) if intersection occurred on  left or right side of  rectangle
 */
int check_intersect_orientation(SDL_Rect wall, SDL_Point *pointA,
		SDL_Point *pointB)
{
	line top = {{wall.x, wall.y}, {wall.x + wall.w, wall.y}};
	line bottom = {{wall.x, wall.y + wall.h}, {wall.x + wall.w, wall.y + wall.h}};
	line ray = {*pointA, *pointB};
	SDL_Point intersection = {0, 0};

	if (lines_intersect(&top, &ray, &intersection))
		return (ORIENT_UP_DOWN);
	else if (lines_intersect(&bottom, &ray, &intersection))
		return (ORIENT_UP_DOWN);
	return (ORIENT_LEFT_RIGHT);
}

/**
 * intersecting_lines - Checks if two line segments intersect
 * @line1: Pointer to line structure representing line1
 * @line2: Pointer to line structure representing line2
 * @hitp: Pointer to SDL point structure indicating the intersection point
 * Internal calculations use fixed-point arithmetic with a 14-bit fractional
 * precision for processors lacking floating-point units
 * Returns: (1) if the lines intersect, (0) if they do not.
 * precision for processors without floating point units.
 * For more explanation visit:-
 * http://paulbourke.net/geometry/pointlineplane/
 *
 * Return: (1) if they intersect, (0) if they do not.
 */
int lines_intersect(line *line1, line *line2, SDL_Point *hitp)
{
	int d, n_a, n_b, ua, ub;

	/* Denominator for ua and ub are the same so store this calculation */
	d = (line2->p2.y - line2->p1.y) * (line1->p2.x - line1->p1.x) -
	(line2->p2.x - line2->p1.x) * (line1->p2.y - line1->p1.y);

	/* n_a and n_b are derived  as seperate values for readability */
	n_a = (line2->p2.x - line2->p1.x) * (line1->p1.y - line2->p1.y) -
	(line2->p2.y - line2->p1.y) * (line1->p1.x - line2->p1.x);

	n_b = (line1->p2.x - line1->p1.x) * (line1->p1.y - line2->p1.y) -
	(line1->p2.y - line1->p1.y) * (line1->p1.x - line2->p1.x);

	/**
	 * Avoid division by zero - this also implies that
	 * the lines are parallel.
	 *
	 * If both n_a and n_b are zero, the lines would be coincident (overlapping).
	 */
	if (d == 0)
		return (0);

	ua = (n_a << 14) / d;
	ub = (n_b << 14) / d;

	if (ua >= 0 && ua <= (1 << 14) && ub >= 0 && ub <= (1 << 14))
	{
		hitp->x = line1->p1.x + ((ua * (line1->p2.x - line1->p1.x)) >> 14);
		hitp->y = line1->p1.y + ((ua * (line1->p2.y - line1->p1.y)) >> 14);
		return (1);
	}

	return (0);
}
