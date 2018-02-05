#include <mlx.h>
#include "libft.h"
#include "keys.h"
#include "wolf3d.h"

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
	view->window = mlx_new_window(view->mlx, 500, 500, "Wolf3d");
	mlx_pixel_put(view->mlx, view->window, 250, 250, 0xFFFFFF);
	mlx_key_hook(view->window, key_handler, view);
	mlx_loop(view->mlx);
	return (0);
}
