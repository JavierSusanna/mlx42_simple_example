#ifndef SPARK_H
# define SPARK_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>
#include <fcntl.h>
# include "../MLX42/include/MLX42/MLX42.h"

#define WIDTH 640
#define HEIGHT 640

typedef struct s_data
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;
	int			cls;
	unsigned char	size;
	uint32_t	mouse_x;
	uint32_t	mouse_y;
	int			buttons;
}	t_data;

#endif
