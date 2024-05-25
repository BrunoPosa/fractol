/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bposa <bposa@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/25 12:08:24 by bposa             #+#    #+#             */
/*   Updated: 2024/05/25 12:16:31 by bposa            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	micro_atoi(char *s)
{
	int	result;

	result = 0;
	if (mystrlen(s) > 4)
		return (ERROR);
	while (*s && (*s >= '0' && *s <= '9') && result < 1200)
		result = result * 10 + *s++ - '0';
	if ((*s && (*s < '0' || *s > '9')) || result > 1200 || result < 50)
		return (ERROR);
	return (result);
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

int	are_digits_in_scope(char *s)
{
	int	i;
	int	temp;

	i = 0;
	temp = 0;
	while (s[i] != '.')
		i++;
	temp = i + 1;
	i++;
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

int	numscheck(char *d1, char *d2)
{
	int	d1_len;
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
	if ((argc != 3 && argc != 5)
		|| (argc == 3 && cmp(argv[1], "mandelbrot") != SUCCESS)
		|| (argc == 5 && cmp(argv[1], "julia") != SUCCESS))
	{
		return (ERROR);
	}
	if (cmp(argv[1], "julia") == SUCCESS
		&& (argv[2][0] == '\0' || argv[3][0] == '\0' || argv[4][0] == '\0'
			|| numscheck(argv[2], argv[3]) == ERROR
				|| micro_atoi(argv[4]) == ERROR))
	{
		return (ERROR);
	}
	if (cmp(argv[1], "mandelbrot") == SUCCESS && micro_atoi(argv[2]) == ERROR)
	{
		return (ERROR);
	}
	return (SUCCESS);
}
