/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 00:47:48 by msamilog          #+#    #+#             */
/*   Updated: 2024/05/31 17:19:44 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strcmp(char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i] || s2[j])
	{
		if (s1[i] == s2[j])
		{
			i++;
			j++;
		}
		else
			return (1);
	}
	return (0);
}

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

void	ft_philo_sleep(t_philo *philo)
{
	philo_print(philo, "is sleeping");
	ft_wait(philo->data->time_to_sleep);
}

int	ft_philo_eat(t_philo *philo)
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
		if (ft_philo_eat(philo))
			break ;
		pthread_mutex_lock(&data->last);
		if (philo->meal_count == data->num_of_must_eat)
			break ;
		pthread_mutex_unlock(&data->last);
		ft_philo_sleep(philo);
		philo_print(philo, "is thinking");
	}
	pthread_mutex_unlock(&data->last);
	return (NULL);
}

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
	int		i;

	data = philo->data;
	if (data->num_of_must_eat != -1)
	{
		i = 0;
		while (i < data->size)
		{
			pthread_mutex_lock(&data->total);
			if (data->philo[i].meal_count < data->num_of_must_eat)
			{
				pthread_mutex_unlock(&data->total);
				return (0);
			}
			pthread_mutex_unlock(&data->total);
			i++;
		}
		return (1);
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

void	ft_free(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->size)
	{
		pthread_mutex_destroy(&data->philo[i].fork);
		i++;
	}
	pthread_mutex_destroy(&data->death);
	pthread_mutex_destroy(&data->last);
	pthread_mutex_destroy(&data->total);
	free(data->philo);
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
