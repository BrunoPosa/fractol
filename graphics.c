#include "./MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define MAXCOUNT 200
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


void	esc_keyhook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(param);
	(void)param;
}

// void	zoomer(char i_to_zoom_in_o_out, t_zoom *zoom)
// {
// 	if (i_to_zoom_in_o_out == 'i' || i_to_zoom_in_o_out == 'I')
// 	{
// 		pixel->x *= 1.003125;
// 		pixel->y *= 1.003125;
// 	}
// 	else
// 	{
// 		pixel->x *= 0.3125;
// 		pixel->y *= 0.3125;
// 	}
// }

// void	scroll_hook(double xdelta, double ydelta, void *param)
// {
// 	if (ydelta > 0)
// 	{
// 		*(double *)param *= 0.8;
// 	}
// 	else if (ydelta < 0)
// 	{
// 		*(double *)param *= 1.2;
// 	}
// 	(void)xdelta;
// }

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
 
int	my_pixel_putter(mlx_image_t *img, t_pixel_xy *pixel, double *zoomf)
{
	int color  = 0x000000FF; 
	int	temp = 0;
	// pixel->x = pixel->x / 1920 * *zoomf;
	// pixel->y = pixel->y / 1920 * *zoomf;

	while (pixel->x < 1920)
	{
		pixel->y = 0;
		while (pixel->y < 1280)
		{
			temp = if_mandelbrot(pixel_to_plane('x', pixel->x) * *(zoomf) - 1.2, pixel_to_plane('y', pixel->y) * *(zoomf));
			if (temp != 0)
				color = temp * MAXCOUNT / 5 + 0xfff300;
			else
				color = 0x000000FF;
			mlx_put_pixel(img, pixel->x, pixel->y, color);
			pixel->y++;
		}
		pixel->x++;
	}
	return (0);
}

int	main(void)
{
	mlx_t	*mlx;
	t_pixel_xy	pixel;
	double	zoomf = 0.025;

	pixel.x = 0;
	pixel.y = 0;
	if (!(mlx = mlx_init(1920, 1280, "MLX42", false)))
		return (EXIT_FAILURE);
	mlx_key_hook(mlx, &esc_keyhook, mlx);
	// mlx_scroll_hook(mlx, &scroll_hook, &zoom_factor);
	mlx_image_t* img = mlx_new_image(mlx, 1920, 1280);
	if (!img)
		return -1;
	// memset(img->pixels, 255, 1 * sizeof(int32_t));
	mlx_image_to_window(mlx, img, 0, 0);
	my_pixel_putter(img, &pixel, &zoomf);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return 0;
}
