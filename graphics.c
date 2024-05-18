#include "./MLX42/include/MLX42/MLX42.h"
#define WINSIZE 1280
#define MAXITER 100
#define SUCCESS 0
#define ERROR -1

typedef struct	s_redraw
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	double		zoomf;
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
		((t_draw *)param)->zoomf *= 0.8;
	}
	else if (ydelta < 0)
	{
		((t_draw *)param)->zoomf *= 1.2;
	}
	renderer(param);
	(void)xdelta;
}

//make this work with different WINSIZE
double	scaled(int n)
{
	return (n * 0.003125 - 2);
}

int	my_color(int i)
{
	if (i != 0)
		return (i * MAXITER / 10 + 0xfff300);
	else
		return (0x000000FF);
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
	while (real * real + imagi * imagi < 4 && i < MAXITER)
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

void	renderer(t_draw *d)
{
	int		x;
	int		y;
	int		i;

	x = 0;
	y = 0;
	i = 0;
	while (x < WINSIZE)
	{
		y = 0;
		while (y < WINSIZE)
		{
			i = mandelbrot(scaled(x) * d->zoomf - 0.75, scaled(y) * d->zoomf);
			mlx_put_pixel(d->img, x, y, my_color(i)); // d->img->pixels[y * WINSIZE + x] = i * (i / 2);
			y++;
		}
		x++;
	}
	mlx_image_to_window(d->mlx, d->img, 0, 0); // ?
}

// free(), mlx_terminate() on errors
int	main(void)
{
	t_draw	draw;

	draw.zoomf = 1.0;
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
