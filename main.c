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

float	find_xintercept_dist(float x_delta, float y_delta, t_map *map)
{
	float currentY;
	float x;
	
	currentY = roundf(map->cameraY);
	if (y_delta < 0)
	{
		while (1)
		{
			x = (currentY - map->cameraY) / (y_delta / x_delta) + map->cameraX;
			if (!in_map((int)x, (int)currentY - 1, map))
				return (0);
			if (map->grid[(int)currentY - 1][(int)x] == '1')
			{
				return fabs(map->cameraY - currentY); // TODO: use hypotenuse to get this work with different camera directions!!!!!
			}
			currentY--;
		}
	}
	// TODO: handle y_delta >= 0
	return (0);
}

float	find_yintercept_dist(float x_delta, float y_delta, t_map *map)
{
	float currentX;
	float y;
	int adjustment;
	
	adjustment = (x_delta < 0) ? 1 : 0;
	if (x_delta < 0)
		currentX = roundf(map->cameraX);
	else
		currentX = roundf(map->cameraX + 1);
	while (1)
	{
		y = (currentX - map->cameraX) * (y_delta / x_delta) + map->cameraY;
		if (!in_map((int)currentX - adjustment, (int)y, map)) // TODO pass map
			return (0);
		ft_putstr("x: ");
		ft_putnbr((int)currentX - adjustment);
		ft_putstr(", y: ");
		ft_putnbr((int)y);
		ft_putchar('\n');
		if (map->grid[(int)y][(int)currentX - adjustment] == '1')
		{
			return fabs(map->cameraY - y); // TODO: use hypotenuse to get this work with different camera directions!!!!!
		}
		if (x_delta < 0)
			currentX--;
		else
			currentX++;
	}
	return (0);
}

float	find_distance(int x, t_map *map, int pixel_width)
{
	float rise;
	float run;

	float dist1;
	float dist2;
	rise = -1 * (pixel_width / 2) / TAN_33; // FIX: multiplying by neg one so direction goes up
	run = -1 * pixel_width / 2 + x;
	
	dist1 = find_xintercept_dist(run, rise, map);
	dist2 = find_yintercept_dist(run, rise, map);
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

	x = 0;
	width_in_pixels = WINDOW_WIDTH * PIXELS_PER_UNIT;
	while (x < width_in_pixels)
	{
		distance = find_distance(x, view->map, width_in_pixels);
		if (x == 0)
			printf("cameraX: %f, cameraY: %f\n", view->map->cameraX, view->map->cameraY);
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

	map = (t_map*)ft_memalloc(sizeof(t_map));
	map->width = 9;
	map->height = 10;
	map->cameraX = 3;
	map->cameraY = 8;
	map->directionX = 0;
	map->directionY = -1;
	table = ft_init_chartable(10, 11);
	table[0] = "111111111";
	table[1] = "100000001";
	table[2] = "100000001";
	table[3] = "100010001";
	table[4] = "100000001";
	table[5] = "100000001";
	table[6] = "100000001";
	table[7] = "100000001";
	table[8] = "100000001";
	table[9] = "111111111";
	table[10] = 0;
	map->grid = table;
	view->map = map;
}

int main(void)
{
	t_view *view;

	view->mlx = mlx_init();
	view->window = mlx_new_window(view->mlx, WINDOW_WIDTH * PIXELS_PER_UNIT, WINDOW_HEIGHT * PIXELS_PER_UNIT, "Wolf3d");
	mlx_key_hook(view->window, key_handler, view);
	parse_map(view);
	draw(view);
	mlx_loop(view->mlx);
	return (0);
}
