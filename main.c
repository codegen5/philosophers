/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 00:47:48 by msamilog          #+#    #+#             */
/*   Updated: 2024/06/01 14:10:46 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, int ac, char **av)
{
	int	i;

	data->size = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->start_time = get_time();
	data->num_of_must_eat = -1;
	data->sim_end = 0;
	if (ac == 6)
		data->num_of_must_eat = ft_atoi(av[5]);
	pthread_mutex_init(&data->death, NULL);
	pthread_mutex_init(&data->last, NULL);
	pthread_mutex_init(&data->total, NULL);
	data->philo = malloc(sizeof(t_philo) * data->size);
	i = 0;
	while (i < data->size)
	{
		data->philo[i].id = i + 1;
		data->philo[i].meal_count = 0;
		data->philo[i].last_meal = data->start_time;
		pthread_mutex_init(&data->philo[i].fork, NULL);
		data->philo[i].data = data;
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
	ft_free(&data);
	return (0);
}
