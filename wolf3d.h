/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scamargo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/05 13:28:19 by scamargo          #+#    #+#             */
/*   Updated: 2018/02/10 18:22:21 by scamargo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H

# define WOLF3D_H

typedef struct	s_map
{
	char	**grid;
	int		width;
	int		height;
	float	cameraX;
	float	cameraY;
	int		directionX;
	int		directionY;
}				t_map;

typedef struct	s_view
{
	void	*mlx;
	void	*window;
	t_map	*map;
}				t_view;

# define PIXELS_PER_UNIT 300
# define WINDOW_HEIGHT 1.15
# define WINDOW_WIDTH 1.15
# define WALL_HEIGHT 4
# define CAMERA_TO_WALL_HEIGHT .5
# define DISTANCE_TO_SCREEN 1
# define TAN_33 0.64940759319
#endif
