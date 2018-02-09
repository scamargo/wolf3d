#include <mlx.h>
#include "libft.h"
#include "keys.h"
#include "wolf3d.h"
#include <stdio.h>
#include <math.h>

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
	while (y > eye_level_height - draw_height)
	{
		if (y < 0)
			break;
		mlx_pixel_put(view->mlx, view->window, x, y, 0xFFFFFF);
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

float	find_xintercept_dist(float x_delta, float y_delta, float camera_pos[2], char **map)
{
	float currentY;
	float x;
	
	currentY = roundf(camera_pos[1]);
	if (y_delta < 0)
	{
		while (1)
		{
			if (1) // TODO: change to if(in_map(currentY))
			{
				x = (currentY - camera_pos[1]) / (y_delta / x_delta) + camera_pos[0];
				if (map[(int)currentY - 1][(int)x] == '1')
				{
					return fabs(camera_pos[1] - currentY); // TODO: use hypotenuse to get this work with different camera directions!!!!!
				}
				currentY--;
			}
			else
				return (0);
		}
	}
	// TODO: handle y_delta >= 0
	return (0);
}

int		in_map(int x, int y) // TODO: use boundaries of map to do check
{
	if (x < 0 || x > 8)
		return (0);
	if (y < 0 || y > 9)
		return (0);
	return (1);
}

float	find_yintercept_dist(float x_delta, float y_delta, float camera_pos[2], char **map)
{
	float currentX;
	float y;
	int adjustment;
	
	adjustment = (x_delta < 0) ? 1 : 0;
	if (x_delta < 0)
		currentX = roundf(camera_pos[0]);
	else
		currentX = roundf(camera_pos[0] + 1);
	//if (x_delta < 0)
	//{
		while (1)
		{
				y = (currentX - camera_pos[0]) * (y_delta / x_delta) + camera_pos[1];
				if (!in_map((int)currentX - adjustment, (int)y))
					return (0);
				if (map[(int)y][(int)currentX - adjustment] == '1')
				{
					return fabs(camera_pos[1] - y); // TODO: use hypotenuse to get this work with different camera directions!!!!!
				}
				if (x_delta < 0)
					currentX--;
				else
					currentX++;
		}
	//}
	// TODO: handle y_delta >= 0
	return (0);
}

float	find_distance(int x, char **map, int pixel_width)
{
	float camera_pos[2];
	float rise;
	float run;

	float dist1;
	float dist2;
	camera_pos[0] = 6;
	camera_pos[1] = 8;
	rise = -1 * (pixel_width / 2) / TAN_30; // FIX: multiplying by neg one so direction goes up
	run = -1 * pixel_width / 2 + x;
	
	dist1 = find_xintercept_dist(run, rise, camera_pos, map);
	dist2 = find_yintercept_dist(run, rise, camera_pos, map);
	if (!dist1)
		return (dist2);
	else if (!dist2)
		return (dist1);
	printf("slope: %f, dist1: %f, dist2: %f\n", rise/run,  dist1, dist2);
	return (dist1 < dist2 ? dist1 : dist2);
	//find_yintercept_dist(
	
	/*if (fabs(rise) > fabs(run))
	{
		run /= fabs(rise);
		rise /= fabs(rise);
	}
	else
	{
		rise /= fabs(run);
		run /= fabs(run);
	}
	i = 0;
	while (1)
	{
		
		// TODO: handle if 1 is never found
		camera_pos[0] += run;
		camera_pos[1] += rise;
		ft_putstr("camera_x: ");
		ft_putnbr(camera_pos[0]);
		ft_putchar('\n');
		ft_putstr("camera_y: ");
		ft_putnbr(camera_pos[1]);
		ft_putchar('\n');

		if (map[(int)camera_pos[1]][(int)camera_pos[0]] == '1')
		{
			
			printf("camera_x: %f, camera_y: %f\n", camera_pos[0], camera_pos[1]);
			return (fabs(rise) * i);
			// if going up and right
			//return (rise / run * (int)camera_pos[0]);
		}
		i++;
	}*/
}


void	draw(t_view *view)
{
	int x;
	int width_in_pixels;
	float distance;
	char **map;

	map = ft_init_chartable(10, 11);
	map[0] = "111111111";
	map[1] = "100111001";
	map[2] = "100000001";
	map[3] = "100000001";
	map[4] = "100000001";
	map[5] = "100000001";
	map[6] = "100000001";
	map[7] = "100000001";
	map[8] = "100000001";
	map[9] = "111111111";
	map[10] = 0;


	x = 0;
	width_in_pixels = WINDOW_WIDTH * PIXELS_PER_UNIT;
	while (x <= width_in_pixels) //should it be less than?
	{
		distance = find_distance(x, map, width_in_pixels);
		draw_column(view, x, distance);
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
	draw(view); // TODO: pass it a map
	mlx_loop(view->mlx);
	return (0);
}
