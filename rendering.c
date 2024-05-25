/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:11:39 by bposa             #+#    #+#             */
/*   Updated: 2024/05/25 12:16:34 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

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
	if (d->fractal == 'j')
		d->limit += 150 * (1.0 - zoomf);
	else
		d->limit += 105 * (1.0 - zoomf);
	while (++i < WINSIZE)
		d->dx[i] *= zoomf;
	while (i-- > 0)
		d->dy[i] *= zoomf;
	renderer(param);
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

void	renderer(t_draw *d)
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
			i = d->fractalf(d->dx[x], d->dy[y], d);
			((uint32_t *)d->img->pixels)[y * WINSIZE + x] = my_color(i);
			y++;
		}
		x++;
	}
}
