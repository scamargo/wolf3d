/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: scamargo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/02/05 13:28:19 by scamargo          #+#    #+#             */
/*   Updated: 2018/02/06 21:02:43 by scamargo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H

# define WOLF3D_H

typedef struct	s_view
{
	void	*mlx;
	void	*window;
}				t_view;

# define PIXELS_PER_UNIT 375
# define WINDOW_HEIGHT 1.15
# define WINDOW_WIDTH 1.15
# define WALL_HEIGHT 4
# define CAMERA_TO_WALL_HEIGHT .5
# define DISTANCE_TO_SCREEN 1
# define TAN_30 0.57735026919
#endif
