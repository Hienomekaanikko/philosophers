/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:19:55 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/27 15:54:41 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long long timestamp(time_t starting_time)
{
	struct timeval	current_time;
	long long		current_time_ms;
	long long		elapsed_ms;

	gettimeofday(&current_time, NULL);
	current_time_ms = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
	elapsed_ms = current_time_ms - starting_time;

	return (elapsed_ms);
}

time_t	init_time(void)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	return((start.tv_sec * 1000) + (start.tv_usec / 1000));
}

int	is_dead(void *arg, time_t comparison_point)
{
	t_philosopher	*philo;

	philo = (t_philosopher*)arg;
	if (timestamp(philo->starting_time) - comparison_point >= philo->time_to_die)
	{
		printf("Time: %lld, Philosopher %d died\n", timestamp(philo->starting_time), philo->id);
		return (1);
	}
	return (0);
}

void	*run_philo(void* arg)
{
	t_philosopher	*philo;
	int				meals;

	philo = (t_philosopher*)arg;
	meals = 0;
	while(!philo->dead)
	{
		if (philo->max_meals > 0)
		{
			if (meals == philo->max_meals)
				break ;
		}
		if (is_dead(&philo, philo->last_meal))
			philo->dead = 1;
		else
		{
			printf("Time: %lld, Philosopher %d is thinking\n", timestamp(philo->starting_time), philo->id);
			pthread_mutex_lock(philo->left_fork);
			philo->left_fork_pickup_time = timestamp(philo->starting_time);
			if (is_dead(&philo, philo->left_fork_pickup_time))
			{
				philo->dead = 1;
				continue ;
			}
			printf("Time: %lld, Philosopher %d has taken the fork %p\n", timestamp(philo->starting_time), philo->id, philo->left_fork);
			pthread_mutex_lock(philo->right_fork);
			printf("Time: %lld, Philosopher %d has taken the fork %p\n", timestamp(philo->starting_time), philo->id, philo->right_fork);
			printf("Time: %lld, Philosopher %d is eating for %dth time\n", timestamp(philo->starting_time), philo->id, meals + 1);
			meals++;
			usleep(philo->time_to_eat * 1000);
			pthread_mutex_unlock(philo->left_fork);
			printf("Time: %lld, Philosopher %d has put down fork %p\n", timestamp(philo->starting_time), philo->id, philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			printf("Time: %lld, Philosopher %d has put down fork %p\n", timestamp(philo->starting_time), philo->id, philo->right_fork);
			philo->last_meal = timestamp(philo->starting_time);
			printf("Time: %lld, Philosopher %d is sleeping\n", timestamp(philo->starting_time), philo->id);
			usleep(philo->time_to_sleep * 1000);
		}
	}
	pthread_mutex_destroy(philo->left_fork);
	pthread_mutex_destroy(philo->right_fork);
	return (NULL);
}

int	init_left_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].left_fork = malloc(sizeof(pthread_mutex_t));
		if (!data->philosophers[i].left_fork)
			return (0);
		pthread_mutex_init(data->philosophers[i].left_fork, NULL);
		i++;
	}
	return (1);
}

void	assign_right_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		if (i + 1 < data->nbr_of_philosophers)
			data->philosophers[i].right_fork = data->philosophers[i + 1].left_fork;
		else
			data->philosophers[i].right_fork = data->philosophers[0].left_fork;
		i++;
	}
}

int init_threads(t_data *data, int argc, char **argv)
{
	int		i;
	time_t	starting_time;

	i = 0;
	starting_time = init_time();
	if (init_left_forks(data))
		assign_right_forks(data);
	data->philosophers->max_meals = 0;
	while (i < data->nbr_of_philosophers)
	{
		if (argc == 6)
			data->philosophers[i].max_meals += ft_atoi(argv[5]);
		data->philosophers[i].last_meal = 0;
		data->philosophers[i].starting_time = starting_time;
		data->philosophers[i].dead = 0;
		data->philosophers[i].time_to_die = ft_atoi(argv[2]);
		data->philosophers[i].time_to_eat = ft_atoi(argv[3]);
		data->philosophers[i].time_to_sleep = ft_atoi(argv[4]);
		if (pthread_create(&data->philosophers[i].thread, NULL, run_philo, &data->philosophers[i]) != 0)
		{
			printf("Thread creation failed for philosopher %d\n", data->philosophers[i].id);
			return (0);
		}
		i++;
	}
	return (1);
}

void	wait_for_finish(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread, NULL);
		i++;
	}
}

	//arg1 number_of_philosophers (same as the  amount of forks)
	//arg2 time_to_die (in ms)
	//arg3 time_to_eat (in ms)
	//arg4 time_to_sleep (in ms)
	//arg5 optional argument [number_of_times_each_philosopher_must_eat]

void	init_data(t_data *data)
{
	data->nbr_of_philosophers = 0;
	data->philosophers = NULL;
	data->error = 0;
}

int check_fork_existance(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		// check which fork is needed by whom
		// printf("Philosopher %d: ", data->philosophers[i].id);
		// printf("left fork address: %p ", data->philosophers[i].left_fork);
		// printf("right fork address: %p\n", data->philosophers[i].right_fork);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_data			data;

	if (argc >= 5 && argc <= 6)
	{
		init_data(&data);
		if (!validate_input(&data, argv))
			return (1);
		data.nbr_of_philosophers = ft_atoi(argv[1]);
		data.philosophers = malloc(data.nbr_of_philosophers * sizeof(t_philosopher));
		if (!data.philosophers)
		{
			error_message("ERROR: Malloc fail!", NULL);
			return (1);
		}
		init_threads(&data, argc, argv);
		check_fork_existance(&data);
		wait_for_finish(&data);
		free_all(&data);
	}
	return (0);
}
