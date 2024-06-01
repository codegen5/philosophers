/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 13:30:06 by msamilog          #+#    #+#             */
/*   Updated: 2024/06/01 14:08:07 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_print(t_philo *philo, char *status)
{
	pthread_mutex_lock(&philo->data->death);
	if (!philo->data->sim_end)
	{
		printf("%ld %d %s\n", (get_time() - philo->data->start_time),
			philo->id, status);
		if (!ft_strcmp(status, "died"))
			philo->data->sim_end = 1;
	}
	pthread_mutex_unlock(&philo->data->death);
}

void	ft_wait(time_t time)
{
	time_t	start;

	start = get_time();
	while (get_time() - start < time)
		usleep(100);
}

void	philo_sleep(t_philo *philo)
{
	philo_print(philo, "is sleeping");
	ft_wait(philo->data->time_to_sleep);
}

int	philo_eat(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&philo->fork);
	philo_print(philo, "has taken a fork");
	if (data->size == 1)
	{
		ft_wait(data->time_to_die);
		philo_print(philo, "died");
		pthread_mutex_unlock(&philo->fork);
		return (1);
	}
	pthread_mutex_lock(&data->philo[(philo->id) % data->size].fork);
	philo_print(philo, "has taken a fork");
	philo_print(philo, "is eating");
	pthread_mutex_lock(&data->last);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&data->last);
	ft_wait(data->time_to_eat);
	pthread_mutex_lock(&data->total);
	philo->meal_count++;
	pthread_mutex_unlock(&data->total);
	pthread_mutex_unlock(&philo->fork);
	pthread_mutex_unlock(&data->philo[(philo->id) % data->size].fork);
	return (0);
}
