#include "./MLX42/include/MLX42/MLX42.h"
#define WINSIZE 800
#define MAXITER 400
#define SUCCESS 0
#define ERROR -1

typedef struct	s_redraw
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	double		zoom;
	double		scale_coef;
	double		dy[WINSIZE];
	double		dx[WINSIZE];
}	t_draw;

void	renderer(t_draw *d);

void	esc_keyhook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(param);
	(void)param;
}

void	scroll_hook(double xdelta, double ydelta, void *param)
{
	if (ydelta > 0)
	{
		((t_draw *)param)->zoom *= 0.9;
	}
	else if (ydelta < 0)
	{
		((t_draw *)param)->zoom *= 1.1;
	}
	renderer(param);
	(void)xdelta;
}

double	scaled(int n, t_draw *d)
{
	return (n * d->scale_coef - 2);
}

int	my_color(int i)
{
	return (i << 24 | i << 16 | (i + i) << 8 | 255);//return (i * i + 1689389995);
}

int	mandelbrot(double x, double y)
{
	int		i;
	double	real;
	double	imagi;
	double	temp;

	i = 0;
	real = 0.0;
	imagi = 0.0;
	while (i < MAXITER && real < 2.0 && imagi < 2.0) // real * real + imagi * imagi < 4
	{
		temp = real * real - imagi * imagi + x;
		imagi = 2 * real * imagi + y;
		real = temp;
		i++;
	}
	if (i < MAXITER)
		return (i);
	return (1);
}

// TODO store y values for WINSIZE in fixed-size array and just iterate over them passing array[i++] to mandelbrot f.
// to lessen multiplications, kind of like why i have an xtemp variable.
// zooms into the seahorse valley (offsets x:-0.743643887037151, and y:0.13182590420533)
void	renderer(t_draw *d)
{
	int			x;
	int			y;
	int			i;
	double		xof;
	double		yof;

	x = 0;
	y = 0;
	i = 0;
	xof = - 0.743643887037249;
	yof = 0.13182590420533;
	while (x < WINSIZE)
	{
		y = 0;
		while (y < WINSIZE)
		{
			i = mandelbrot(d->dx[x] * d->zoom + xof, d->dy[y] * d->zoom + yof);
			((uint32_t *)d->img->pixels)[y * WINSIZE + x] = my_color(i); // TODO could this use i++ to access array elems? instead of y * and + operations?
			y++;
		}
		x++;
	}
}

// free(), mlx_terminate() on errors
int	main(void)
{
	t_draw	draw;
	int		i;

	i = -1;
	draw.zoom = 1.0;
	draw.scale_coef = 4.0 / WINSIZE;
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
	renderer(&draw);
	mlx_key_hook(draw.mlx, &esc_keyhook, draw.mlx);
	mlx_scroll_hook(draw.mlx, &scroll_hook, &draw);
	mlx_loop(draw.mlx);
	mlx_terminate(draw.mlx);
	return (SUCCESS); 
}
