#include "./MLX42/include/MLX42/MLX42.h"
#define WINSIZE 600
#define SUCCESS 0
#define ERROR -1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_redraw
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			limit;
	double		scale_coef;
	double		dy[WINSIZE];
	double		dx[WINSIZE];
	char		fractal;
	double		jreal;
	double		jimag;
}	t_draw;

int		mandelbrot(long double x, long double y, t_draw *d);
void	mandel_render(t_draw *d);
int		julia(long double x, long double y, t_draw *d);
void	julia_render(t_draw *d);
int		my_color(int i);
void	esc_keyhook(mlx_key_data_t keydata, void *param);
void	scroll_hook(double xdelta, double ydelta, void *param);

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
	d->limit += 105 * (1.0 - zoomf);
	while (++i < WINSIZE)
		d->dx[i] *= zoomf;
	while (i-- > 0)
		d->dy[i] *= zoomf;
	if (d->fractal == 'm')
		mandel_render(param);
	else
		julia_render(param);
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
	y = y + 0.12172377388442482241;
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

void	julia_render(t_draw *d) // no need for two render functions, just pass the correct function in the struct
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
			i = julia(d->dx[x], d->dy[y], d);
			((uint32_t *)d->img->pixels)[y * WINSIZE + x] = my_color(i);
			y++;
		}
		x++;
	}
}

int	julia(long double x, long double y, t_draw *d)
{
	int			i;
	long double	real;
	long double	imagi;
	long double	temp;

	i = 0;
	real = x;
	imagi = y;
	temp = 0.0;
	while (i < d->limit && real < 2.0 && imagi < 2.0)
	{
		temp = real * real - imagi * imagi + d->jreal;
		imagi = 2 * real * imagi + d->jimag;
		real = temp;
		i++;
	}
	if (i < d->limit)
		return (i);
	return (1);
}

int	cmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	if (!s1 || !s2)
		return (-1);
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] - s2[i] != 0)
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	mystrlen(char *s)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	my_isdigit(char c)
{
	if (c >= '0' && c <= '9')
		return ('y');
	return ('n');
}

int	are_digits_in_scope(char *s)
{
	int	i;
	int temp;

	i = 0;
	temp = 0;
	while (s[i] != '.')
		i++;
	temp = i++;
	while (s[i] != '\0')
	{
		if (my_isdigit(s[i]) == 'n')
			return (ERROR);
		i++;
	}
	if (i - temp > 10)
		return (ERROR);
	return (SUCCESS);
}

double	my_atod(char *s)
{
	double	n;
	int		i;
	double	sign;
	double	divisor;
	double	int_part;

	n = 0.0;
	i = 0;
	sign = 1.0;
	divisor = 1.0;
	int_part = 0.0;
	if (s[i] == '-')
	{
		sign = -1.0;
		i++;
	}
	int_part = (double)(s[i++] - '0');
	while (s[++i] != '\0')
	{
		divisor *= 10.0;
		n = n + (double)(s[i] - '0') / divisor;
	}
	n += int_part;
	return (sign * n);
}

int	numscheck(char *d1, char *d2)
{
	int d1_len;
	int	d2_len;

	d1_len = mystrlen(d1);
	d2_len = mystrlen(d2);
	if (d1_len < 3 || d2_len < 3 || d1_len > 13 || d2_len > 13
		|| (d1[0] == '-' && d1_len < 4) || (d2[0] == '-' && d1_len < 4))
		return (ERROR);
	if ((d1[0] != '-' && d1[0] != '0' && d1[0] != '1' && d1[0] != '2')
		|| (d2[0] != '-' && d2[0] != '0' && d2[0] != '1' && d2[0] != '2'))
		return (ERROR);
	if ((d1[0] == '-' && d1[2] != '.') || (d2[0] == '-' && d2[2] != '.'))
		return (ERROR);
	if ((my_isdigit(d1[0]) == 'y' && d1[1] != '.')
		|| (my_isdigit(d2[0]) == 'y' && d2[1] != '.'))
		return (ERROR);
	if (are_digits_in_scope(d1) == ERROR || are_digits_in_scope(d2) == ERROR)
		return (ERROR);
	if (my_atod(d1) > 2.0 || my_atod(d1) < -2.0
		|| my_atod(d2) > 2.0 || my_atod(d2) < -2.0)
		return (ERROR);
	return (SUCCESS);
}

int	validator(int argc, char *argv[])
{
	if ((argc != 2 && argc != 4)
		|| (argc == 2 && cmp(argv[1], "mandelbrot") != SUCCESS)
			|| (argc == 4 && cmp(argv[1], "julia") != SUCCESS))
	{
		return (ERROR);
	}
	if (cmp(argv[1], "julia") == SUCCESS
		&& ((argv[2][0] == '\0' || argv[3][0] == '\0')
			|| numscheck(argv[2], argv[3]) == ERROR))
	{
		return (ERROR);
	}
	return (SUCCESS);
}

int	initializor(char **argv, t_draw *d)
{
	int	i;

	i = -1;
	d->fractal = argv[1][0];

	d->scale_coef = 4.0 / WINSIZE;
	d->limit = 600.0;
	d->jreal = 0.0;
	d->jimag = 0.0;
	if (d->fractal == 'j')
	{
		d->jreal = my_atod(argv[2]);
		d->jimag = my_atod(argv[3]);
	}
	while (++i < WINSIZE)
		d->dx[i] = i * d->scale_coef - 2;
	while (i-- > 0)
		d->dy[i] = i * d->scale_coef - 2;
	d->mlx = mlx_init(WINSIZE, WINSIZE, "fractol", false);
	if (!(d->mlx))
		return (ERROR);
	d->img = mlx_new_image(d->mlx, WINSIZE, WINSIZE);
	if (!d->img || mlx_image_to_window(d->mlx, d->img, 0, 0) < 0)
		return (ERROR);
	return (SUCCESS);
}

int	input_error(int rvalue)
{
	if (write(1, "***********************************************\n", 48)
	+ write(1, "*  Error: Invalid input!                      *\n", 48)
	+ write(1, "*                                             *\n", 48)
	+ write(1, "*  Usage examples:                            *\n", 48)
	+ write(1, "*   ./fractol julia -0.4 0.59                 *\n", 48)
	+ write(1, "*   ./fractol mandelbrot                      *\n", 48)
	+ write(1, "*                                             *\n", 48)
	+ write(1, "*  Note: For 'julia', provide 2 parameters    *\n", 48)
	+ write(1, "*       in the range -2.0 to 2.0,             *\n", 48)
	+ write(1, "*       with 1 to 10 decimals.                *\n", 48)
	+ write(1, "*                                             *\n", 48)
	+ write(1, "*  Please try again.                          *\n", 48)
	+ write(1, "***********************************************\n", 48) != 624)
	{
		return (ERROR);
	}
	return (rvalue);
}

int	error_exit(mlx_t *mlx)
{
	mlx_terminate(mlx);
	return (ERROR);
}

int	main(int argc, char *argv[])
{
	t_draw	draw;

	if (validator(argc, argv) == ERROR)
		return (input_error(ERROR));
	if (initializor(argv, &draw) == ERROR)
		return (error_exit(draw.mlx));
	if (draw.fractal == 'm')
		mandel_render(&draw); //renderer(&draw); w/ function pointer in struct?
	else if (draw.fractal == 'j')
		julia_render(&draw);
	mlx_key_hook(draw.mlx, &esc_keyhook, draw.mlx);
	mlx_scroll_hook(draw.mlx, &scroll_hook, &draw);
	mlx_loop(draw.mlx);
	mlx_terminate(draw.mlx);
	return (SUCCESS); 
}
