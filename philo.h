/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msamilog <tahasamiloglu@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 00:41:27 by msamilog          #+#    #+#             */
/*   Updated: 2024/05/30 20:50:36 by msamilog         ###   ########.fr       */
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
	pthread_mutex_t	fork;
	struct s_data	*data;
}		t_philo;

typedef struct s_data
{
	int				size;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	time_t			start_time;
	int				num_of_must_eat;
	int				sim_end;
	t_philo			*philo;
	pthread_mutex_t	death;
	pthread_mutex_t	last;
	pthread_mutex_t	total;
}				t_data;

#endif