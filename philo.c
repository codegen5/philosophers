#include "philo.h"

time_t get_time(void)
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

int ft_atoi(char *str)
{
    int i;
    long result;

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

void ft_wait(time_t time)
{
    time_t start;

    start = get_time();
    while (get_time() - start < time)
    {
        usleep(100);
    }
}

void print_status(t_philo *philo, char *status)
{
    pthread_mutex_lock(&philo->data->print_mutex);
    if (!philo->data->sim_end)
        printf("%ld %d %s\n", get_time() - philo->data->start_time, philo->id, status);
    pthread_mutex_unlock(&philo->data->print_mutex);
}

void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    t_data *data = philo->data;

    if (data->size == 1)
    {
        pthread_mutex_lock(&philo->fork);
        print_status(philo, "has taken a fork");
        while (1)
        {
            pthread_mutex_lock(&data->check_dead);
            if (data->sim_end)
            {
                pthread_mutex_unlock(&data->check_dead);
                break;
            }
            pthread_mutex_unlock(&data->check_dead);
            usleep(100);
        }
        pthread_mutex_unlock(&philo->fork);
        return (NULL);
    }

    while (1)
    {
        pthread_mutex_lock(&data->check_dead);
        if (data->sim_end)
        {
            pthread_mutex_unlock(&data->check_dead);
            break;
        }
        pthread_mutex_unlock(&data->check_dead);

        pthread_mutex_lock(&philo->fork);
        print_status(philo, "has taken a fork");
        pthread_mutex_lock(&data->philos[philo->id % data->size].fork);
        print_status(philo, "has taken a fork");

        print_status(philo, "is eating");
        pthread_mutex_lock(&data->check_dead);
        philo->last_meal = get_time();
        philo->meal_count++;
        pthread_mutex_unlock(&data->check_dead);
        ft_wait(data->time_to_eat);
        pthread_mutex_unlock(&data->philos[philo->id % data->size].fork);
        pthread_mutex_unlock(&philo->fork);

        print_status(philo, "is sleeping");
        ft_wait(data->time_to_sleep);

        print_status(philo, "is thinking");
    }
    return (NULL);
}

void start_threads(t_philo *philos)
{
    int i;

    i = 0;
    while (i < philos->data->size)
    {
        pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]);
        usleep(50);
        i++;
    }
}

void join_threads(t_philo *philos)
{
    int i;

    i = 0;
    while (i < philos->data->size)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
}

void init_data(t_data *data, int ac, char **av)
{
    int i;

    data->size = ft_atoi(av[1]);
    data->time_to_die = ft_atoi(av[2]);
    data->time_to_eat = ft_atoi(av[3]);
    data->time_to_sleep = ft_atoi(av[4]);
    data->start_time = get_time();
    data->num_of_must_eat = 0;
    data->sim_end = 0;
    data->death_printed = 0;
    if (ac == 6)
        data->num_of_must_eat = ft_atoi(av[5]);
    pthread_mutex_init(&data->check_dead, NULL);
    pthread_mutex_init(&data->print_mutex, NULL);
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

int args_valid(int ac, char **av)
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

void main_thread(t_data *data)
{
    int i;

    while (1)
    {
        i = 0;
        while (i < data->size)
        {
            pthread_mutex_lock(&data->check_dead);
            if (get_time() - data->philos[i].last_meal > data->time_to_die)
            {
                if (!data->death_printed)
                {
                    print_status(&data->philos[i], "died");
                    data->death_printed = 1;
                }
                data->sim_end = 1;
                pthread_mutex_unlock(&data->check_dead);
                return;
            }
            pthread_mutex_unlock(&data->check_dead);
            i++;
        }
        usleep(1000);
    }
}

int main(int ac, char **av)
{
    t_data data;

    if (!args_valid(ac, av))
        return (1);
    init_data(&data, ac, av);
    start_threads(data.philos);
    main_thread(&data);
    join_threads(data.philos);
    return (0);
}
