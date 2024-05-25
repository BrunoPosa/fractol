/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:01:23 by bposa             #+#    #+#             */
/*   Updated: 2024/05/25 12:16:32 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include "./MLX42/include/MLX42/MLX42.h"
# include <unistd.h>
# define WINSIZE 600
# define SUCCESS 0
# define ERROR -1

typedef struct s_redraw
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	int			limit;
	double		scale_coef;
	double		dy[WINSIZE];
	double		dx[WINSIZE];
	char		fractal;
	int			(*fractalf)(long double, long double, struct s_redraw *);
	double		jreal;
	double		jimag;
}	t_draw;

void	initializor(char **argv, t_draw *d);
int		input_error(int rvalue);
int		error_exit(mlx_t *mlx);
void	esc_keyhook(mlx_key_data_t keydata, void *param);
int		validator(int argc, char *argv[]);
int		numscheck(char *d1, char *d2);
int		are_digits_in_scope(char *s);
double	my_atod(char *s);
int		micro_atoi(char *s);
void	renderer(t_draw *d);
int		mandelbrot(long double x, long double y, t_draw *d);
int		julia(long double x, long double y, t_draw *d);
void	scroll_hook(double xdelta, double ydelta, void *param);
int		my_color(int i);
char	my_isdigit(char c);
int		mystrlen(char *s);
int		cmp(char *s1, char *s2);

#endif