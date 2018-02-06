#include <mlx.h>
#include "libft.h"
#include "keys.h"
#include "wolf3d.h"
#include <stdio.h>

void	draw_column(t_view *view, int x)
{
	float distance_to_wall;
	float fov_height;
	float wall_over_fov;
	int window_height_pixels;
	int eye_level_height;
	int y;
	float draw_height;
	
	distance_to_wall = 12;
	fov_height = WINDOW_HEIGHT * distance_to_wall / DISTANCE_TO_SCREEN;
	wall_over_fov = WALL_HEIGHT / fov_height;
	window_height_pixels = WINDOW_HEIGHT * PIXELS_PER_UNIT;
	eye_level_height = window_height_pixels / 2;
	y = eye_level_height;
	draw_height = wall_over_fov * WINDOW_HEIGHT * (1 - CAMERA_TO_WALL_HEIGHT) * PIXELS_PER_UNIT;
	while (y > eye_level_height - draw_height)
	{
		if (y < 0)
			break;
		mlx_pixel_put(view->mlx, view->window, x, y, 0xFFFFFFF);
		y--;
	}
	y = eye_level_height;
	draw_height = wall_over_fov * WINDOW_HEIGHT * CAMERA_TO_WALL_HEIGHT * PIXELS_PER_UNIT;
	while (y < eye_level_height + draw_height)
	{
		if (y > window_height_pixels)
			break;
		mlx_pixel_put(view->mlx, view->window, x, y, 0xFFFFFF);
		y++;
	}
}

void	draw(t_view *view)
{
	int x;
	int width_in_pixels;

	x = 0;
	width_in_pixels = WINDOW_WIDTH * PIXELS_PER_UNIT;
	while (x <= width_in_pixels)
	{
		draw_column(view, x);
		x++;
	}
}

int key_handler(int key, t_view *view)
{
	if (key == KEY_ESC)
		ft_putendl("escape");
	mlx_pixel_put(view->mlx, view->window, 10, 10, 0xFFFFFF);
	return (0);
}

int main(void)
{
	t_view *view;

	view->mlx = mlx_init();
	view->window = mlx_new_window(view->mlx, WINDOW_WIDTH * PIXELS_PER_UNIT, WINDOW_HEIGHT * PIXELS_PER_UNIT, "Wolf3d");
	mlx_key_hook(view->window, key_handler, view);
	draw(view); // TODO: pass it a map & screen dimensions
	mlx_loop(view->mlx);
	return (0);
}
