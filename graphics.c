#include "./MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define MAXCOUNT 500
typedef struct s_pixel_xy_values
{
	int	x;
	int	y;
}	t_pixel_xy;

typedef struct s_zoom_values
{
	double	x;
	double	y;
}	t_zoom;

typedef struct s_redraw_loop_packet
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_pixel_xy	*pixel;
	double		*zoomf;
}	t_redraw_pack;


void	esc_keyhook(mlx_key_data_t keydata, void *param);
void	scroll_hook(double xdelta, double ydelta, void *param);
double	pixel_to_plane(char axis, int pixel_value);
int		if_mandelbrot(double x, double y);
void	my_pixel_putter(mlx_image_t *img, t_pixel_xy *pixel, double *zoomf);




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
		*((t_redraw_pack *)param)->zoomf *= 0.8;
	}
	else if (ydelta < 0)
	{
		*((t_redraw_pack *)param)->zoomf *= 1.2;
	}
	my_pixel_putter(((t_redraw_pack *)param)->img,
		((t_redraw_pack *)param)->pixel, ((t_redraw_pack *)param)->zoomf);
	(void)xdelta;
}

/*
	pixel_to_plane converts pixel_value x or y into double values
	scaled to be relevant for a fractal, between -2 and 2.
	scaled_value = (pix_value * max_pix_value / max_scale_value) - scale_offset
	[scale_offset is used to move the scaled '0' value towards window center]
*/
double	pixel_to_plane(char axis, int pixel_value)
{
	if (axis == 'x' || axis == 'X')
		return (pixel_value * 0.003125 - 4);
	return (pixel_value * 0.003125 - 2);
}
/*
	X and Y are real and imaginary parts of the complex number, respectively.
	Imaginary part is Y * sqrt(-1). When the sqroot of -1 is squared, then Y
	becomes -Y. 
	explanation++
*/
int	if_mandelbrot(double x, double y)
{
	int		count;
	double	real;
	double	imagi;
	double	temp;

	count = 0;
	real = 0.0;
	imagi = 0.0;
	while (real * real + imagi * imagi < 4 && count < MAXCOUNT)
	{
		temp = real * real - imagi * imagi + x;
		imagi = 2 * real * imagi + y;
		real = temp;
		count++;
	}
	if (count < MAXCOUNT)
		return (count);
	return (0);
}


// int	colorer(mlx_t *mlx, int i, int j)
// {
// 	double	zoom_factor;

// 	zoom_factor = 1.0;

// 	if (zoom_factor != 1.0)
// 		printf("MORE\n");
// 	if (if_mandelbrot(pixel_to_plane('x', i) * zoom_factor, pixel_to_plane('y', j) * zoom_factor) == 0)
// 		return (0x000000FF);
// 	return (0xFFFFFFFF);
// }
 
void	my_pixel_putter(mlx_image_t *img, t_pixel_xy *pixel, double *zoomf)
{
	int color  = 0x000000FF;
	int	temp = 0;
	pixel->x = pixel->x / 1920 * *zoomf;
	pixel->y = pixel->y / 1920 * *zoomf;

	while (pixel->x < 1920)
	{
		pixel->y = 0;
		while (pixel->y < 1280)
		{
			temp = if_mandelbrot(pixel_to_plane('x', pixel->x) * *(zoomf), pixel_to_plane('y', pixel->y) * *(zoomf));
			if (temp != 0)
				color = temp * MAXCOUNT / 5 + 0xfff300;
			else
				color = 0x000000FF;
			mlx_put_pixel(img, pixel->x, pixel->y, color);
			pixel->y++;
		}
		pixel->x++;
	}
}

int	main(void)
{
	mlx_t	*mlx;
	t_pixel_xy	pixel;
	double	zoomf = 0.5;
	t_redraw_pack	redraw;

	pixel.x = 0;
	pixel.y = 0;
	if (!(mlx = mlx_init(1920, 1280, "fract", false)))
		return (EXIT_FAILURE);
	mlx_image_t* img = mlx_new_image(mlx, 1920, 1280);
	redraw.mlx = mlx;
	redraw.img = img;
	redraw.pixel = &pixel;
	redraw.zoomf = &zoomf;
	mlx_key_hook(mlx, &esc_keyhook, mlx);
	mlx_scroll_hook(mlx, &scroll_hook, &redraw);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		return -1;
	// memset(img->pixels, 255, 1 * sizeof(int32_t));
	my_pixel_putter(img, &pixel, &zoomf);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return 0;
}
