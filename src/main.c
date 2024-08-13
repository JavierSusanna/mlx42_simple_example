#include "../include/spark.h"

int32_t pixel_select(int32_t r, int32_t g, int32_t b, int32_t a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

float	randf(void)
{
	u_int32_t	rnd_int;

	rnd_int = (rand() & ((1 << 23) - 1)) | (1 << 30);
	return (-3.0f + *((float *) &rnd_int));
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
}

void loop_hook_function(void *param)
{
	t_data		*data;
	uint32_t	dx;
	uint32_t	dy;
	int	ct;
	
	data = (t_data *)param;
	if (data->pause)
		return ;
	ct = 0;
	while (++ct < (1 << 16))
	{
/*		col = (((u_int32_t) rand() << 8) | (255U)); */
		data->color = (1536 + data->color + (rand() % 3) - 1) % 1536;
		if (mlx_is_mouse_down(data->mlx, MLX_MOUSE_BUTTON_LEFT))
		{
			mlx_get_mouse_pos(data->mlx, &dx, &dy);
			dx -= data->x;
			if (dx < - WIDTH / 2)
				dx += WIDTH;
			else if (dx > WIDTH / 2)
				dx -= WIDTH;
			if (dy < - HEIGHT / 2)
				dy += HEIGHT;
			else if (dy > HEIGHT / 2)
				dy -= HEIGHT;
		}
		dx = 2 * rand() / RAND_MAX - 1;
		dy = 2 * rand() / RAND_MAX - 1;
		data->x += dx;
		if (data->x < 0)
			data->x += WIDTH;
		data->y += dy;
		if (data->y < 0)
			data->y += HEIGHT;
		mlx_put_pixel(data->img, data->x, data->y, rainbow(data->color));
	}
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
	data.x = WIDTH / 2;
	data.y = HEIGHT / 2;
	data.color = rand() % 1536;
	data.pause = 1;
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
