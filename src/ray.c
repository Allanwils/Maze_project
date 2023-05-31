#include "../headers/maze.h"

/**
 * rotate_point - rotates an SDL_Point around an arbitrary center by @deg
 * degrees
 *@point: SDL_Point data structure representing the Cartesian point to rotate
 *@cx: x coordinate of the arbitrary center
 *@cy: y coordinate of the arbitrary center
 *@deg: Degrees to rotate the point (converted to radians)
 *@ray_size: Length of the casted ray, which increases based on collision encounters
 *Returns: New SDL_Point after rotation
 */
SDL_Point rotate_point(const SDL_Point *point, float cx, float cy,
		float deg, float ray_size)
{
	SDL_Point new_point = {0, 0};

	/**
	 * For details on how this operates, refer to
	 * https://danceswithcode.net/engineeringnotes/rotations_in
	 * _2d/rotations_in_2d.html
	 * Adjust our point along the y-axis by the @ray_size, which represents
	 * the length of the ray
	 */
	new_point.x = ((point->x - cx) * cos(deg) - (((point->y - ray_size) - cy) *
				sin(deg))) + cx;
	new_point.y = ((point->x - cx) * sin(deg) + (((point->y - ray_size) - cy) *
				cos(deg))) + cy;

	return (new_point);
}

/**
 *shoot_rays - casts rays onto the 2D map
 *@sdl: sdl_instance structure
 *@player: player data structure
 *@map: map_t data structure containing map information
 *@map_active: Boolean indicating map visibility
 *Returns: void
 */
void raycast(sdl_instance *sdl, player *player, map_t *map,
		SDL_bool *map_active)
{
	SDL_Point point, center;
	double i, deg = (player->view_angle - (FOV / 2.0));
	double angleBtwnRays;
	double ray_length = 0.0;
	double correct_distance = 0.0;
	SDL_Color orientation_color = {0, 0, 0, 0};
	int orientation;
	double ang = (deg - FOV);

	/* Center means  player's center position coordinates */
	center.x = player->locale.x + (player->locale.w / 2);
	center.y = player->locale.y + (player->locale.h / 2);
	angleBtwnRays = ((FOV) / (SCREEN_WIDTH * 1.0));

	for (i = 0; i < SCREEN_WIDTH; i++)
	{
		/* Change  degree to radian, rotate point by degree from center */
		point = check_ray_intersections(&center, ang, *map, &ray_length,
				&orientation);
		shade_walls(&orientation_color, orientation);
		/* Drawing of  rays on 2D map */
		if (*map_active)
		{
			REND_COLOR_GREEN(sdl->renderer);
			SDL_RenderDrawLine(sdl->renderer, center.x, center.y, point.x, point.y);
		}
		correct_distance = remove_fish_eye_effect(player, ray_length, ang);
		draw_3D_walls(sdl, correct_distance, i, orientation_color);

		ang += angleBtwnRays;
	}
}

/**
 *detect_ray_intersections - examines ray-wall intersections
 *@center: pointer to point originating from player center
 *@ray_rotation_angle: rotation angle of the point
 *@map: map_t data structure containing map information
 *@ray_len: pointer to double for storing resulting ray length
 *@orientation: Side where the ray intersects. (1) up/down
 *(2) left/right
 *Returns: SDL Point with x and y coordinates of the ray
 */
SDL_Point check_ray_intersections(SDL_Point *center, double ray_rotation_angle,
map_t map, double *ray_len, int *orientation)
{
	SDL_Point point = {center->x, center->y};
	int i, j;
	SDL_Rect wall = {0, 0, GRID_SIZE, GRID_SIZE};
	line line = {{0, 0}, {0, 0}};
	SDL_bool is_intersecting = SDL_FALSE;
	double op, adj, hy;

	point = rotate_point(&point, center->x, center->y, RADIAN(ray_rotation_angle),
			MAX_DRAW_DISTANCE);
	for (i = 0; i < map.rows; i++)
	{
		for (j = 0; j < map.columns; j++)
		{
			if (map.arr[i][j] == '0')
				continue;
			wall.x = (j << 4) + MAP_MARGIN;
			wall.y = (i << 4) + MAP_MARGIN;

			line.p1 = *center;
			line.p2 = point;
			is_intersecting = SDL_IntersectRectAndLine(&wall,
			&line.p1.x, &line.p1.y, &line.p2.x, &line.p2.y);

			if (is_intersecting == SDL_TRUE)
			{
				*orientation = check_intersect_orientation(wall, center,
						&line.p1);
				adj = line.p1.x - center->x;
				op = line.p1.y - center->y;
				hy = sqrt((pow(op, 2.0) + pow(adj, 2.0)));

				point = *center;
				point = rotate_point(&point, center->x, center->y,
						RADIAN(ray_rotation_angle), hy);
				*ray_len = hy;
			}
		}
	}
	return (point);
}

/**
 *fix_fish_eye_effect - eliminates the distortion that causes lines to appear oval-like
 *@player: player data structure containing player information
 *@ray_length: length of the ray from player center to the wall intersection point
 *@ray_view_angle: current angle of the ray relative to the viewing angle
 *Description: Also referred to as the Fishbowl effect, it occurs due to the combination
 *of polar and Cartesian coordinates in the ray-casting implementation.
 *Returns: Corrected length of the ray after removing the viewing distortion
 */
double remove_fish_eye_effect(player *player, double ray_length,
		double ray_view_angle)
{
	double distorted_distance = ray_length;
	double correct_distance = 0.0;
	double deg = (player->view_angle - (FOV / 2.0));

	/**
	 * The distorted_distance creates a fish-eye effect.
	 * To obtain the accurate distance, we multiply the distorted
	 * distance by the cosine of the angle between the casted ray and the viewing angle.
	 */

	correct_distance = distorted_distance *
		cos(RADIAN((ray_view_angle - (deg - FOV / 2.0))));

	return (correct_distance);
}

/**
 *render_3D_walls - draws lines on the SDL renderer
 *@sdl: sdl_instance data structure
 *@ray_length: Normalized length of the ray (difference between 
 *ray's y position and player's y position)
 *@ray_index: Current screen column to draw the ray
 *@wall_color: Pointer to SDL_Color data structure representing the wall color
 *Returns: void
 */
void draw_3D_walls(sdl_instance *sdl, double ray_length, int ray_index,
	SDL_Color wall_color)
{
	int d_to_projection_plane = (SCREEN_WIDTH / 2) / (tan(RADIAN((FOV / 2))));
	double line_height = ceil((SCREEN_HEIGHT / (ray_length * 1.0)) *
		(d_to_projection_plane >> 6));
	double draw_start = (SCREEN_HEIGHT / 2.0) - (line_height / 2.0);
	/**
	 * if (draw_start < 0)
	 * draw_start = 0;
	 * if (draw_end >= SCREEN_HEIGHT)
	 * draw_end = SCREEN_HEIGHT - 1.0;
	 */
	/**
	 * REND_COLOR(sdl->renderer, wall_color.r, wall_color.g, wall_color.b,
	 * wall_color.a);
	 */
	texture_walls(sdl, line_height, ray_index, draw_start, wall_color.a);
	/**
	 * SDL_RenderDrawLine(sdl->renderer, ray_index, draw_start, ray_index,
	 * draw_end);
	 */
}
