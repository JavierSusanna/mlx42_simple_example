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
	data->cls = 0;
	ini = (uint32_t *)data->img->pixels;
	i = -1;
	while (++i < WIDTH * HEIGHT)
		*(ini + i) = (u_int32_t) (255U << 24);
}
int	biased_rand(int distance, int range, unsigned char sz)
{
	t_data		*data;
	uint32_t	r;

	range = range * sz / 24;
	if (distance + range < 0)
		distance += 2 * range;
	else if (distance > range)
		distance -= 2 * range;
	r = rand() + RAND_MAX / 3 / range * (range + distance);
	r /= RAND_MAX / 3;
	return ((int) r - 2);
}

int is_in_window(int x, int y, int width, int height)
{
	return (x >= 0 && x < width && y >= 0 && y < height);
}

void loop_hook_function(void *param)
{
	t_data		*data;
	int			tmp;
	int	ct;
	
	data = (t_data *)param;
	mlx_get_mouse_pos(data->mlx, &(data->mouse_x), &(data->mouse_y));
	data->buttons = mlx_is_mouse_down(data->mlx, MLX_MOUSE_BUTTON_LEFT);
	if (is_in_window(data->mouse_x, data->mouse_y, WIDTH, HEIGHT) == false)
		return ;
	if (!data->buttons)
	{
		if (data->cls)
			reset_img(data);
		return ;
	}
	data->x = data->mouse_x;
	data->y = data->mouse_y;
	ct = 0;
	while (++ct < (1 << 10))
	{
/*		col = (((u_int32_t) rand() << 8) | (255U)); */
		data->color = (1536 + data->color + 4 * ((rand() % 3) - 1)) % 1536;
		tmp = 0;
		if (data->buttons)
			tmp = data->mouse_x - data->x;
		data->x = (WIDTH + data->x + biased_rand(tmp, WIDTH / 2, data->size)) % WIDTH;
		tmp = 0;
		if (data->buttons)
			tmp = data->mouse_y - data->y;
		data->y = (HEIGHT + data->y + biased_rand(tmp, HEIGHT / 2, data->size)) % HEIGHT;
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
			data->cls = 1;
		if (keydata.key == MLX_KEY_UP && data->size < 128)
			data->size *= 2;
		if (keydata.key == MLX_KEY_DOWN && data->size > 3)
			data->size /= 2;
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

	data.mlx = mlx_init(WIDTH, HEIGHT, "Brownian Noise Paint", true);
	data.img = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	data.x = WIDTH / 2;
	data.y = HEIGHT / 2;
	data.color = rand() % 1536;
	data.cls = 1;
	data.size = 12;
	reset_img(&data);
	printf("\n*********************************\n");
	printf("  Brownian Motion Art.\n");
	printf("  Paint with the mouse.\n");
	printf("  Press SPACE to CLEAR SCREEN.\n");
	printf("  Press UP / DOWN arrows to make the brush bigger / smaller.\n");
	printf("*********************************\n");
	mlx_image_to_window(data.mlx, data.img, 0, 0);
	mlx_loop_hook(data.mlx, loop_hook_function, &data);
	mlx_key_hook(data.mlx, &ft_on_key, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	return 0;
}
