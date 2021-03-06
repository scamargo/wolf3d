#include <mlx.h>
#include "libft.h"
#include "keys.h"
#include "wolf3d.h"
#include <stdio.h>
#include <math.h>

int		in_map(int x, int y, t_map *map)
{
	if (x < 0 || x > map->width - 1)
		return (0);
	if (y < 0 || y > map->height - 1)
		return (0);
	return (1);
}

void	draw_column(t_view *view, int x, float distance_to_wall)
{
	float fov_height;
	float wall_over_fov;
	int window_height_pixels;
	int eye_level_height;
	int y;
	float draw_height;
	
	fov_height = WINDOW_HEIGHT * distance_to_wall / DISTANCE_TO_SCREEN;
	wall_over_fov = WALL_HEIGHT / fov_height;
	window_height_pixels = WINDOW_HEIGHT * PIXELS_PER_UNIT;
	eye_level_height = window_height_pixels / 2;
	y = eye_level_height;
	draw_height = wall_over_fov * WINDOW_HEIGHT * (1 - CAMERA_TO_WALL_HEIGHT) * PIXELS_PER_UNIT;
	while (y >= 0)
	{
		if (y > eye_level_height - draw_height)
			mlx_pixel_put(view->mlx, view->window, x, y, 0xFFFFFF);
		else
			mlx_pixel_put(view->mlx, view->window, x, y, 0x000000);
		y--;
	}
	y = eye_level_height;
	draw_height = wall_over_fov * WINDOW_HEIGHT * CAMERA_TO_WALL_HEIGHT * PIXELS_PER_UNIT;
	while (y < window_height_pixels)
	{
		if (y < eye_level_height + draw_height)
			mlx_pixel_put(view->mlx, view->window, x, y, 0xFFFFFF);
		else
			mlx_pixel_put(view->mlx, view->window, x, y, 0x000000);
		y++;
	}
}

float	find_xintercept_dist(float x_delta, float y_delta, t_map *map, double angle)
{
	float currentY;
	float x;
	int adjustment;
	double hypotenuse;

	adjustment = (y_delta < 0) ? 1 : 0;
	currentY = (int)(y_delta < 0 ? map->cameraY : map->cameraY + 1); 
	while (1)
	{
		x = (currentY - map->cameraY) / (y_delta / x_delta) + map->cameraX;
		if (!in_map((int)x, (int)currentY - adjustment, map))
			return (0);
		if (map->grid[(int)currentY - adjustment][(int)x] == '1')
		{
			hypotenuse = sqrt(pow(x - map->cameraX, 2) + pow(currentY - map->cameraY, 2));
			//return (get_true_distance(map->plane_dir, y_delta/x_delta, hypotenuse));
			return (hypotenuse * cos(angle));
		}
		currentY += y_delta < 0 ? -1 : 1;
	}
	// TODO: handle y_delta >= 0
	return (0);
}

float	find_yintercept_dist(float x_delta, float y_delta, t_map *map, double angle)
{
	float currentX;
	float y;
	int adjustment;
	double hypotenuse;
	
	adjustment = (x_delta < 0) ? 1 : 0;
	if (x_delta < 0)
		currentX = (int)map->cameraX;
	else
		currentX = (int)map->cameraX + 1;
	while (1)
	{
		if (!x_delta)
			return (0);
		y = (currentX - map->cameraX) * (y_delta / x_delta) + map->cameraY;
		if (!in_map((int)currentX - adjustment, (int)y, map))
					return (0);
		if (map->grid[(int)y][(int)currentX - adjustment] == '1')
		{
			hypotenuse = sqrt(pow(y - map->cameraY, 2) + pow(currentX - map->cameraX, 2));
			return (hypotenuse * cos(angle));
		}
		if (x_delta < 0)
			currentX--;
		else
			currentX++;
	}
	return (0);
}

void	rotate(float *p_y, float *p_x, double rotation)
{
	float x;
	float y;

	y = *p_y;
	x = *p_x;
	*p_y = x * sin(rotation) + y * cos(rotation);
	*p_x = x * cos(rotation) - y * sin(rotation);
}

float	find_distance(int x, t_map *map, int pixel_width)
{
	float rise;
	float run;

	float dist1;
	float dist2;
	double angle;

	rise = (pixel_width / 2) / TAN_33;
	run = -1 * pixel_width / 2 + x; // TODO: should this always be negative??????
	//printf("pre rise: %f, run: %f\n", rise, run);
	rotate(&rise, &run, map->camera_dir); // TODO: test
	angle = fabs(run) / (pixel_width / 2) * 33 * (M_PI / 180);
	dist1 = find_xintercept_dist(run, rise, map, angle);
	dist2 = find_yintercept_dist(run, rise, map, angle);
	//printf("rot rise: %f, run: %f\n", rise, run);
	if (!dist1)
		return (dist2);
	else if (!dist2)
		return (dist1);
	return (dist1 < dist2 ? dist1 : dist2);
}

void	draw(t_view *view)
{
	int x;
	int width_in_pixels;
	float distance;

	printf("location x: %f, y: %f\n", view->map->cameraX, view->map->cameraY);
	x = 0;
	width_in_pixels = WINDOW_WIDTH * PIXELS_PER_UNIT;
	while (x < width_in_pixels)
	{
		distance = find_distance(x, view->map, width_in_pixels);
		printf("distance: %f\n", distance);
		draw_column(view, x, distance);
		x++;
	}
}

int key_handler(int key, t_view *view)
{
	if (key == KEY_ESC)
		ft_putendl("escape");
	else if (key == KEY_W)
	{
		view->map->cameraY += -.1; // TODO: adjust cameraX & cameraY based on direction
		draw(view);
	}
	else if (key == KEY_S)
	{
		view->map->cameraY += .1; // TODO: adjust cameraX & cameraY based on direction
		draw(view);
	}
	else if (key == KEY_A)
	{
		view->map->cameraX += -.1; // TODO: adjust based on camera position
		draw(view);
	}
	else if (key == KEY_D)
	{
		view->map->cameraX += .1;
		draw(view);
	}	
	return (0);
}

void	parse_map(t_view *view)
{
	t_map *map;
	char **table;
	double input_dir = M_PI/ 2;

	map = (t_map*)ft_memalloc(sizeof(t_map));
	map->width = 42;
	map->height = 10;
	map->cameraX = 4.5;
	map->cameraY = 5;
	map->camera_dir = 0;
	map->plane_dir = map->camera_dir + M_PI / 2;
	table = ft_init_chartable(10, 43);
	table[0] = "111111111111111111111111111111111111111111";
	table[1] = "100000000000000000000000000000000000000001";
	table[2] = "100000000000000000000000000000000000000001";
	table[3] = "100000000000000000000000000000000000000001";
	table[4] = "100000000000000000000000000000000000000001";
	table[5] = "100000000000000000000000000000000000000001";
	table[6] = "100000000000000000000000000000000000000001";
	table[7] = "100000000000000000000000000000000000000001";
	table[8] = "100000000000000000000000000000000000000001";
	table[9] = "111111111111111111111111111111111111111111";
	table[10] = 0;
	map->grid = table;
	view->map = map;
}

int main(void)
{
	t_view *view;
	double time;
	double oldTime;

	time = 0;
	oldTime = 0;
	view->mlx = mlx_init();
	view->window = mlx_new_window(view->mlx, WINDOW_WIDTH * PIXELS_PER_UNIT, WINDOW_HEIGHT * PIXELS_PER_UNIT, "Wolf3d");
	mlx_key_hook(view->window, key_handler, view);
	parse_map(view);
	draw(view);
	mlx_loop(view->mlx);
	return (0);
}
