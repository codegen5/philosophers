/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 00:47:48 by msamilog          #+#    #+#             */
/*   Updated: 2024/05/29 21:52:24 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

time_t	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
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

void	init_data(t_data *data, int ac, char **av)
{
	int	i;

	data->size = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->start_time = get_time();
	data->num_of_must_eat = 0;
	data->sim_end = 0;
	if (ac == 6)
		data->num_of_must_eat = ft_atoi(av[5]);
	pthread_mutex_init(&data->check_dead, NULL);
	data->philos = malloc(sizeof(t_philo) * data->size);
	i = 0;
	while (i < data->size)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meal_count = 0;
		data->philos[i].last_meal = data->start_time;
		pthread_mutex_init(&data->philos[i].fork, NULL);
		data->philos[i].data = data;
		i++;
	}
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
	t_data	data;

	if (!args_valid(ac, av))
		return (1);
	init_data(&data, ac, av);
	create_threads(&data);
	return (0);
}
