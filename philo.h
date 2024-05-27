/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 00:41:27 by msamilog          #+#    #+#             */
/*   Updated: 2024/05/27 16:49:53 by msamilog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_philo
{
	int				id;
	int				meal_count;
	time_t			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*forks;
}		t_philo;

typedef struct s_data
{
	int		size;
	time_t	time_to_die;
	time_t	time_to_eat;
	time_t	time_to_sleep;
	int		num_of_must_eat;
}				t_data;

int	ft_atoi(char *str);

#endif