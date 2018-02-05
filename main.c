#include <mlx.h>
#include "libft.h"
#include "keys.h"

int key_handler(int key, void *param)
{
	if (key == KEY_ESC)
		ft_putendl("escape");
	//mlx_pixel_put(param[0], param[1], 10, 10, 0xFFFFFF);
	return (0);
}

int main(void)
{
	void *mlx;
	void *window;

	mlx = mlx_init();
	//params[0] = mlx;
	window = mlx_new_window(mlx, 500, 500, "Wolf3d");
	//params[1] = window;
	mlx_pixel_put(mlx, window, 250, 250, 0xFFFFFF);
	mlx_key_hook(window, key_handler, (void *)0);
	mlx_loop(mlx);
	return (0);
}
