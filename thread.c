/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 13:30:08 by msamilog          #+#    #+#             */
/*   Updated: 2024/06/01 14:08:07 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death);
	if ((get_time() - philo->last_meal) > philo->data->time_to_die)
	{
		pthread_mutex_unlock(&philo->data->death);
		philo_print(philo, "died");
		return (1);
	}
	pthread_mutex_unlock(&philo->data->death);
	return (0);
}

int	is_served(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	if (data->num_of_must_eat != -1)
	{
		pthread_mutex_lock(&data->total);
		if (philo->meal_count == data->num_of_must_eat)
		{
			pthread_mutex_unlock(&data->total);
			return (1);
		}
		pthread_mutex_unlock(&data->total);
	}
	return (0);
}

int	check_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->size)
	{
		if (is_dead(&data->philo[i]) || is_served(&data->philo[i]))
			return (1);
		i++;
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(&data->death);
		if (data->sim_end)
		{
			pthread_mutex_unlock(&data->death);
			break ;
		}
		pthread_mutex_unlock(&data->death);
		if (philo_eat(philo))
			break ;
		pthread_mutex_lock(&data->last);
		if (philo->meal_count == data->num_of_must_eat)
			break ;
		pthread_mutex_unlock(&data->last);
		philo_sleep(philo);
		philo_print(philo, "is thinking");
	}
	pthread_mutex_unlock(&data->last);
	return (NULL);
}

void	create_threads(t_data *data)
{
	int		i;
	t_philo	*philo;

	philo = data->philo;
	i = 0;
	while (i < data->size)
	{
		if (pthread_create(&philo[i].thread, NULL, philo_routine, &philo[i]))
			return ;
		usleep(50);
		i++;
	}
	while (1)
	{
		if (check_simulation(data))
			break ;
		usleep(100);
	}
	i = 0;
	while (i < data->size)
	{
		pthread_join(philo[i].thread, NULL);
		i++;
	}
}
