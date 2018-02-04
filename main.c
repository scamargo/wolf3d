#include <mlx.h>
#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

int key_handler(int key, void *param)
{
	ft_putchar('X');
	return (0);
}

int main(void)
{
	void *mlx;
	void *window;

	mlx = mlx_init();
	window = mlx_new_window(mlx, 500, 500, "Wolf3d");
	mlx_pixel_put(mlx, window, 250, 250, 0xFFFFFF);
	mlx_key_hook(window, key_handler, (void *)0);
	mlx_loop(mlx);
	return (0);
}
