#ifndef STARK_H
# define STARK_H

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
	uint32_t	color;
	int			pause;
}	t_data;

#endif
