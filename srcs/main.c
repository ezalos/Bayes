/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:36:45 by ldevelle          #+#    #+#             */
/*   Updated: 2020/01/16 16:29:58 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define	SIZE	80
#define	LENGTH	(SIZE * 2)
#define	HEIGHT	SIZE
#define OFFSET	1

typedef struct		s_bayes
{
	int				a;
	int				b;
	int				n;
	long double		exp[2];
	long double		pos[2];
	long double		hyp[2];
	// long double		guess_y;
	// long double		guess_x;
}					t_bayes;

double factorial (int n) {
    double f[211] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 479001600, 6227020800, 87178291200, 1307674368000, 20922789888000, 355687428096000, 6402373705728000, 121645100408832000, 2432902008176640000/*, 51090942171709440000*/};
	if (n < 21)
		return f[n];
	double solution = 1;
    for (int i = 2; i <= n; i++)
        solution *= i;
    return solution;
}

long double	bayes_distribution(double x, double y, t_bayes *bayes)
{
	// Pr(x, y | Tn) = x^a (1 – x)^(n – a) y^a (1 – y)^(n – y) / Pr(Tn),
	// Pr(Tn) = a! (n – a)! b! (n – b)! / (n – 1)!^2
	long double prob_x;
	long double prob_y;
	long double prob_tn_a;
	long double prob_tn_b;
	long double prob_tn;
	long double result;

	prob_x = pow(x, bayes->a) * pow(1 - x, bayes->n - bayes->a);
	prob_y = pow(y, bayes->b) * pow(1 - y, bayes->n - bayes->b);
	prob_tn_a = factorial(bayes->a) * factorial(bayes->n - bayes->a);
	prob_tn_b = factorial(bayes->b) * factorial(bayes->n - bayes->b);
	prob_tn = prob_tn_a * prob_tn_b / pow(factorial(bayes->n + 1), 2);
	result =  prob_x * prob_y / prob_tn;
	return (result);
}

void	clear_screen(void)
{
	int		j;
	int		i;

	ft_place_cursor(0, 0);
	ft_printf(" ");
	i = -1;
	while (++i < LENGTH)
		ft_printf("_");
	ft_printf("\n");
	j = -1;
	while (++j < HEIGHT)
	{
		ft_printf("|");
		i = -1;
		while (++i < LENGTH)
			ft_printf(" ");
		ft_printf("|\n");
	}
	ft_printf(" ");
	i = -1;
	while (++i < LENGTH)
		ft_printf("_");
	ft_printf("\n");
}

void	plot_point(long double *point, int red, int green, int blue)
{
	ft_place_cursor(OFFSET + 1 + (point[0] * (HEIGHT - 1)),
		OFFSET + 1 + (point[1] * (LENGTH - 1)));
	ft_printf("%~{b*;*;*} %~{}", red, green, blue);
}

void	plot_point_int(int y, int x, int red)
{
	int blue = 0;
	int green = 0;

	if (red < 0)
		red = 0;
	ft_place_cursor(OFFSET + x, OFFSET + y);
	if (red > 255)
	{
		blue = red - 255;
		if (blue < 0)
			blue = 0;
		if (blue > 255)
		{
			green = blue - 255;
			if (green < 0)
				green = 0;
			if (green > 255)
				green = 255;
			blue = 255;
		}
		red = 255;
		// ft_printf(" ");
	}
	// else
		ft_printf("%~{b*;*;*} %~{}", red, green, blue);
}

void 	rerender_points(t_bayes *bayes, int x, int y, int mode)
{
	if (mode || ((int)(bayes->hyp[0] * HEIGHT) == y && (int)(bayes->hyp[1] * LENGTH) == x))
		plot_point(bayes->hyp, 255, 0, 0);
	if (mode || ((int)(bayes->exp[0] * HEIGHT) == y && (int)(bayes->exp[1] * LENGTH) == x))
		plot_point(bayes->exp, 255, 255, 255);
	if (mode || ((int)(bayes->pos[0] * HEIGHT) == y && (int)(bayes->pos[1] * LENGTH) == x))
		plot_point(bayes->pos, 0, 255, 0);

}

int 	plot_bayes(t_bayes *bayes)
{
	static long double	max = 1;
	long double	res;
	int i;
	int j;

	i = -1;
	while (++i < HEIGHT)
	{
		j = -1;
		while (++j < LENGTH)
		{
			res = bayes_distribution((double)i / (double)HEIGHT, (double)j / (double)LENGTH, bayes);
			if (res == INFINITY || res == -INFINITY)
				return (-1);
			(void)max;
			plot_point_int(j + OFFSET,  i + OFFSET, (int)(((double)res / (double)max) * (double)255 * 0.05));
			rerender_points(bayes, i, j, 0);
		}
	}
	return (0);
}

void	init(t_bayes *bayes)
{
	srand((unsigned int)(long long)bayes + rand());
	ft_bzero(bayes, sizeof(t_bayes));
	bayes->pos[0] = (double)rand() / (double)RAND_MAX;
	bayes->pos[1] = (double)rand() / (double)RAND_MAX;
}

int		new_data(t_bayes *bayes, int iter)
{
	static int	save;

	if (!iter)
		save = 0;
	bayes->exp[0] = (double)rand() / (double)RAND_MAX;
	bayes->exp[1] = (double)rand() / (double)RAND_MAX;
	if (bayes->exp[0] < bayes->pos[0])
		bayes->a++;
	if (bayes->exp[1] < bayes->pos[1])
		bayes->b++;
	bayes->n++;
	bayes->hyp[0] = (double)bayes->a / (double)bayes->n;
	bayes->hyp[1] = (double)bayes->b / (double)bayes->n;

	rerender_points(bayes, 0, 0, 1);
	if (save * 1.1 < iter)
	{
		save = iter;
		if (plot_bayes(bayes))
			return (0);
	}
	return (-1);
}

int		main(void)
{
	int				iter;
	t_bayes			bayes;

	while (1)
	{
		init(&bayes);
		iter = -1;
		clear_screen();
		while (new_data(&bayes, ++iter));
	}
	ft_place_cursor(HEIGHT + 10, LENGTH + 4);
	return (0);
}
