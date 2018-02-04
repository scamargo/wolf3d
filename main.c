#include <mlx.h>

int main(void)
{
	void *mlx;
	void *window;

	mlx = mlx_init();
	window = mlx_new_window(mlx, 500, 500, "Wolf3d");

	mlx_loop(mlx);
	return (0);
}
