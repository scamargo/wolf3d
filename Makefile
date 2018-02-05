all:
	###gcc -Wall -Wextra -Werror -I minilibx -L. -lft -lmlx -framework OpenGL -framework AppKit main.c -o wolf3d
	gcc -I minilibx -L. -lft -lmlx -framework OpenGL -framework AppKit main.c -o wolf3d
