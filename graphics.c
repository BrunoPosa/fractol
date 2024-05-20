#include "./MLX42/include/MLX42/MLX42.h"
#define WINSIZE 800
#define SUCCESS 0
#define ERROR -1

typedef struct	s_redraw
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			limit;
	double		scale_coef;
	double		dy[WINSIZE];
	double		dx[WINSIZE];
}	t_draw;

void	mandel_render(t_draw *d);

void	esc_keyhook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(param);
	(void)param;
}

void	scroll_hook(double xdelta, double ydelta, void *param)
{
	t_draw	*d;
	double	zoomf;
	int		i;

	d = (t_draw *)param;
	zoomf = 1.0;
	i = -1;
	if (ydelta > 0)
		zoomf = 0.5;
	else if (ydelta < 0)
		zoomf = 1.5;
	d->limit += 100 * (1.0 - zoomf);
	while (++i < WINSIZE)
		d->dx[i] *= zoomf;
	while (i-- > 0)
		d->dy[i] *= zoomf;
	// julia_render(param);
	mandel_render(param);
	(void)xdelta;
}

int	my_color(int i)
{
	return (i << 24 | i << 16 | 220 << 8 | 255);
}

int	mandelbrot(long double x, long double y, t_draw *d)
{
	int			i;
	long double	real;
	long double	imagi;
	long double	temp;

	i = 0;
	real = 0.0;
	imagi = 0.0;
	temp = 0.0;
	x = x - 0.74453986035590838011;
	y = y + 0.12172377389442482241;
	while (i < d->limit && real < 2.0 && imagi < 2.0)
	{
		temp = real * real - imagi * imagi + x;
		imagi = 2 * real * imagi + y;
		real = temp;
		i++;
	}
	if (i < d->limit)
		return (i);
	return (1);
}

void	mandel_render(t_draw *d)
{
	int	x;
	int	y;
	int	i;

	x = 0;
	y = 0;
	i = 0;
	while (x < WINSIZE)
	{
		y = 0;
		while (y < WINSIZE)
		{
			i = mandelbrot(d->dx[x], d->dy[y], d);
			((uint32_t *)d->img->pixels)[y * WINSIZE + x] = my_color(i);
			y++;
		}
		x++;
	}
}

// free(), mlx_terminate() on errors
// init f()
int	main(void)
{
	t_draw	draw;
	int		i;

	i = -1;
	draw.scale_coef = 4.0 / WINSIZE;
	draw.limit = 600.0;
	while (++i < WINSIZE)
		draw.dx[i] = i * draw.scale_coef - 2;
	while (i-- > 0)
		draw.dy[i] = i * draw.scale_coef - 2;
	draw.mlx = mlx_init(WINSIZE, WINSIZE, "fractol", false);
	if (!(draw.mlx))
		return (ERROR);
	draw.img = mlx_new_image(draw.mlx, WINSIZE, WINSIZE);
	if (!draw.img || mlx_image_to_window(draw.mlx, draw.img, 0, 0) < 0)
		return (ERROR);
	mandel_render(&draw);
	mlx_key_hook(draw.mlx, &esc_keyhook, draw.mlx);
	mlx_scroll_hook(draw.mlx, &scroll_hook, &draw);
	mlx_loop(draw.mlx);
	mlx_terminate(draw.mlx);
	return (SUCCESS); 
}
