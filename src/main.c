#include "../include/stark.h"

int32_t pixel_select(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

uint32_t	rainbow(unsigned int col)
{
	uint8_t		rng;
	uint8_t		step;
	uint32_t	ret;

	col = col % 1536;
	rng = col >> 8;
	step = col & 255;
	ret = 0;
	if (rng == 0)
		ret = 255 | (step << 8);
	if (rng == 1)
		ret = (255 - step) | (255 << 8);
	if (rng == 2)
		ret = (255 << 8) | (step << 16);
	if (rng == 3)
		ret = ((255 - step) << 8) | (255 << 16);
	if (rng == 4)
		ret = step | (255 << 16);
	if (rng == 5)
		ret = 255 | ((255 - step) << 16);
	return (ret << 8 | 255);
}

void reset_img(void *param)
{
	t_data *data;
	uint32_t	*ini;
	long		i;

	data = (t_data *)param;
	ini = (uint32_t *)data->img->pixels;
	i = -1;
	while (++i < WIDTH * HEIGHT)
		*(ini + i) = (u_int32_t) (255U << 24);
/*		*(ini + i) = (u_int32_t) (255U << 24) ((1U << 31) - 1U + (1U << 31)); */
	*((int *) data->img->pixels) = WIDTH / 2;
	*((int *) data->img->pixels + 1) = HEIGHT / 2;
	*((int *) data->img->pixels + 2) = rand() % 1536;
}

void loop_hook_function(void *param)
{
	t_data *data;
	int	x;
	int	y;
	int	ct;
	int	col;
	
	data = (t_data *)param;
	if (data->pause)
		return ;
	x = *((int *) data->img->pixels);
	y = *((int *) data->img->pixels + 1);
	col = *((int *) data->img->pixels + 2);
	ct = 0;
	while (++ct < (1 << 16))
	{
/*		col = (((u_int32_t) rand() << 8) | (255U)); */
		col = (1536 + col + (rand() % 3) - 1) % 1536;
		mlx_put_pixel(data->img, x, y, rainbow(col));
		x = (x + (rand() % 3) - 1 + WIDTH) % WIDTH;
		y = (y + (rand() % 3) - 1 + HEIGHT) % HEIGHT;
	}
	*((int *) data->img->pixels) = x;
	*((int *) data->img->pixels + 1) = y;
	*((int *) data->img->pixels + 2) = col;
}

void ft_on_key(mlx_key_data_t keydata, void *param)
{
	t_data *data;

	data = (t_data *)param;

	//press once
	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_SPACE)
			data->pause ^= 1;
		//close window, exit program
		if (keydata.key == MLX_KEY_ESCAPE)
		{
			mlx_close_window(data->mlx);
			mlx_terminate(data->mlx);
			exit(0);
		}
	}
}

int main(int argc, char **argv)
{
	t_data data;

	data.mlx = mlx_init(WIDTH, HEIGHT, "Golden Sphere", true);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	data.pause = 0;
	reset_img(&data);
	printf("img reset\n");
	mlx_image_to_window(data.mlx, data.img, 0, 0);
	mlx_loop_hook(data.mlx, loop_hook_function, &data);
	printf("loop_hook\n");
	mlx_key_hook(data.mlx, &ft_on_key, &data);
	printf("key_hook\n");
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	return 0;
}
