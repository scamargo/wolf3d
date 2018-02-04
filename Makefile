all:
	###gcc -Wall -Wextra -Werror -I minilibx -L minilibx_macos -lmlx -framework OpenGL -framework AppKit main.c
	gcc -I minilibx -L minilibx_macos -lmlx -framework OpenGL -framework AppKit main.c
