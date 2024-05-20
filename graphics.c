#include "./MLX42/include/MLX42/MLX42.h"
#define WINSIZE 800
#define SUCCESS 0
#define ERROR -1

//convert to long double? slower..
typedef struct	s_redraw
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	long double	zoom;
	long double	limit;
	double		scale_coef;
	double		dy[WINSIZE];
	double		dx[WINSIZE];
}	t_draw;

void	mandel_render(t_draw *d);
//can i get rid of scaled? store it in the zoomed value of struct? basically, I can call it "mapped" and do all needed for it
// that way i save some multiplications
double	scaled(double n, t_draw *d)
{
	return (n * d->scale_coef - 2);//- scaled(d->mxpos, d)
}

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

	d = (t_draw *)param;
	zoomf = 1.0;
	if (ydelta > 0)
		zoomf = 0.5;
	else if (ydelta < 0)
		zoomf = 1.5;
	d->zoom *= zoomf;
	d->limit += 100 * (1.0 - zoomf);
	mandel_render(param);
	(void)xdelta;
}

int	my_color(int i)
{
	return (i << 24 | i << 16 | (i + i) << 8 | 255);
}

int	mandelbr(double x, double y, t_draw *d)
{
	int		i;
	double	real;
	double	imagi;
	double	temp;

	i = 0;
	real = 0.0;
	imagi = 0.0;
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
	int		x;
	int		y;
	int		i;
	double	xof;
	double	yof;

	x = 0;
	y = 0;
	i = 0;
	xof = -0.74453986035590838011;
	yof = 0.12172377389442482241;
	while (x < WINSIZE)
	{
		y = 0;
		while (y < WINSIZE)
		{
			i = mandelbr(d->dx[x] * d->zoom + xof, d->dy[y] * d->zoom + yof, d);
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
	draw.zoom = 1.0;
	draw.scale_coef = 4.0 / WINSIZE;
	draw.limit = 500.0;
	while (++i < WINSIZE)
		draw.dx[i] = scaled(i, &draw);
	while (i-- > 0)
		draw.dy[i] = scaled(i, &draw);
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
