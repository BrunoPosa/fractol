/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:03:24 by bposa             #+#    #+#             */
/*   Updated: 2024/05/26 22:02:41 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	esc_keyhook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(param);
	(void)param;
}

void	initializor(char **argv, t_draw *d)
{
	int	i;

	i = -1;
	d->fractal = argv[1][0];
	d->scale_coef = 4.0 / WINSIZE;
	d->fractalf = &mandelbrot;
	d->jreal = 0.0;
	d->jimag = 0.0;
	d->limit = 0;
	if (d->fractal == 'm')
		d->limit = micro_atoi(argv[2]);
	if (d->fractal == 'j')
	{
		d->fractalf = &julia;
		d->jreal = my_atod(argv[2]);
		d->jimag = my_atod(argv[3]);
		d->limit = micro_atoi(argv[4]);
	}
	while (++i < WINSIZE)
		d->dx[i] = i * d->scale_coef - 2;
	while (i-- > 0)
		d->dy[i] = i * d->scale_coef - 2;
}

int	input_error(int rvalue)
{
	if (write(1, "                                               \n", 48)
		+ write(1, "***********************************************\n", 48)
		+ write(1, "*  Error: Invalid input!                      *\n", 48)
		+ write(1, "*                                             *\n", 48)
		+ write(1, "*  Usage examples:                            *\n", 48)
		+ write(1, "*   ./fractol julia -0.4 0.59 500             *\n", 48)
		+ write(1, "*   ./fractol mandelbrot 600                  *\n", 48)
		+ write(1, "*                                             *\n", 48)
		+ write(1, "*  Note: For 'julia', provide 2 parameters    *\n", 48)
		+ write(1, "*       in the range -2.0 to 2.0,             *\n", 48)
		+ write(1, "*       with 1 to 10 decimals.                *\n", 48)
		+ write(1, "*  Note: Provide fractal start depth for both *\n", 48)
		+ write(1, "*       fractals at end, between 50 and 1200. *\n", 48)
		+ write(1, "*                                             *\n", 48)
		+ write(1, "*  Please try again.                          *\n", 48)
		+ write(1, "***********************************************\n", 48)
		!= 768)
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
	initializor(argv, &draw);
	draw.mlx = mlx_init(WINSIZE, WINSIZE, "fractol", false);
	if (!(draw.mlx))
		return (ERROR);
	draw.img = mlx_new_image(draw.mlx, WINSIZE, WINSIZE);
	if (!draw.img || mlx_image_to_window(draw.mlx, draw.img, 0, 0) < 0)
		return (error_exit(draw.mlx));
	renderer(&draw);
	mlx_key_hook(draw.mlx, &esc_keyhook, draw.mlx);
	mlx_scroll_hook(draw.mlx, &scroll_hook, &draw);
	mlx_loop(draw.mlx);
	mlx_terminate(draw.mlx);
	return (SUCCESS);
}
