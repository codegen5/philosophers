/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 00:47:48 by msamilog          #+#    #+#             */
/*   Updated: 2024/05/27 16:51:16 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, int ac, char **av)
{
	data->size = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->num_of_must_eat = 0;
	if (ac == 6)
		data->num_of_must_eat = ft_atoi(av[5]);
}

int	ft_atoi(char *str)
{
	int		i;
	long	result;

	result = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		result *= 10;
		result = result + str[i] - '0';
		if (result > 2147483647)
			return (0);
		i++;
	}
	return ((int)result);
}

int	args_valid(int ac, char **av)
{
	if (ac == 5 || ac == 6)
	{
		while (--ac)
		{
			if (!ft_atoi(av[ac]))
				return (0);
		}
		return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_philo	philo;
	t_data	data;

	if (!args_valid(ac, av))
		return (1);
	init_data(&data, ac, av);
	printf("size: %d\ntime_to_die: %li\ntime_to_eat: %li\ntime_to_sleep: %li\nnum_of_must_eat: %d\n",
		data.size, data.time_to_die, data.time_to_eat, data.time_to_sleep, data.num_of_must_eat);
	return (0);
}
