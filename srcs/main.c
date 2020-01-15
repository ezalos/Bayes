/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldevelle <ldevelle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 13:36:45 by ldevelle          #+#    #+#             */
/*   Updated: 2020/01/15 16:07:39 by ezalos           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/includes/libft.h"
#include <stdio.h>
#include <stdlib.h>

#define	SIZE	80

typedef struct		s_bayes
{
	char			orig[2];
	char			guess[2];
	char			last[2];
	int				up;
	int				down;
	int				left;
	int				right;
}					t_bayes;

void	print_tab(t_bayes *bayes)
{
	int		j;
	int		i;

	ft_place_cursor(0, 0);
	ft_printf("\n ");
	i = -1;
	while (++i < SIZE)
		ft_printf("_");
	ft_printf("\n");
	j = -1;
	while (++j < SIZE)
	{
		ft_printf("|");
		i = -1;
		while (++i < SIZE)
		{
			if (i == bayes->guess[1] && j == bayes->guess[0])
			{
				ft_printf("%~{b255;0;0}");
				ft_printf("G");
				ft_printf("%~{}");
			}
			else if (i == bayes->orig[1] && j == bayes->orig[0])
			{
				ft_printf("%~{b0;255;0}");
				ft_printf("O");
				ft_printf("%~{}");
			}
			else if (i == bayes->last[1] && j == bayes->last[0])
			{
				ft_printf("%~{b255;255;255}");
				ft_printf("L");
				ft_printf("%~{}");
			}
			else
				ft_printf(" ");
		}
		ft_printf("|\n");
	}
	ft_printf(" ");
	i = -1;
	while (++i < SIZE)
		ft_printf("_");
	ft_printf("\n");
}

void	init(t_bayes *bayes)
{
	srand((unsigned int)(long long)bayes + rand());
	ft_bzero(bayes, sizeof(*bayes));
	bayes->guess[0] = SIZE / 2;
	bayes->guess[1] = SIZE / 2;
	bayes->last[0] = -1;
	bayes->last[1] = -1;
	bayes->orig[0] = rand() % SIZE;
	bayes->orig[1] = rand() % SIZE;
	// ft_printf("[%d;%d]\n", bayes->orig[0], bayes->orig[1]);
}

void		new_data(t_bayes *bayes, int iter)
{
	static intmax_t save = 0;
	static intmax_t waiting = 100000000;

	bayes->last[0] = rand() % SIZE;
	bayes->last[1] = rand() % SIZE;
	if (bayes->last[0] < bayes->orig[0])
		bayes->up++;
	else if (bayes->last[0] > bayes->orig[0])
		bayes->down++;

	if (bayes->last[1] < bayes->orig[1])
		bayes->left++;
	else if (bayes->last[1] > bayes->orig[1])
		bayes->right++;

	bayes->guess[0] = ((float)(bayes->up) / (float)(bayes->up + bayes->down)) * (SIZE);
	bayes->guess[1] = ((float)(bayes->left) / (float)(bayes->left + bayes->right)) * (SIZE);
	bayes->guess[0] >= SIZE ? bayes->guess[0]--:(void)bayes;
	bayes->guess[1] >= SIZE ? bayes->guess[1]--:(void)bayes;
	if (iter < 100 || iter > (float)save * 1.1)
	{
		save = iter;
		print_tab(bayes);
		ft_printf("%-6d\t", iter);
		ft_printf("G[%3d;%3d]\t", bayes->guess[0], bayes->guess[1]);
		ft_printf("L[%3d;%3d]\t", bayes->last[0], bayes->last[1]);
		ft_printf("O[%3d;%3d]\n", bayes->orig[0], bayes->orig[1]);
		ft_printf("U %-5d D %-5d L %-5d R %-5d\n", bayes->up, bayes->down, bayes->left, bayes->right);
		// ft_wait_pls(waiting);
		waiting = (float)waiting * 0.9;
	}
}

int		main(void)
{
	int				iter;
	t_bayes			bayes;

	while (1)
	{
		init(&bayes);
		print_tab(&bayes);
		iter = 0;
		while (iter < 100000)
			new_data(&bayes, ++iter);
	}
	return (0);
}
