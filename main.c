/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:19:55 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/03 13:44:52 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void *run_philo(void *arg)
{
	t_philosopher *philo = (t_philosopher *)arg;

	if (philo->data->nbr_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->left_fork);
		printf("%lld %d has taken a fork\n", timestamp(philo->starting_time), philo->id);
		usleep(philo->time_to_die * 1000);
		pthread_mutex_unlock(&philo->left_fork);
		printf("%lld %d died\n", timestamp(philo->starting_time), philo->id);
		philo->dead = 1;
		philo->data->simulation_running = 0;
		return (NULL);
	}
	while (!philo->dead && philo->data->simulation_running)
	{
		pthread_mutex_lock(&philo->mutex);
		if (!philo->data->simulation_running ||
			timestamp(philo->starting_time) - philo->last_meal >= philo->time_to_die)
		{
			if (philo->data->simulation_running)
				printf("%lld %d died\n", timestamp(philo->starting_time), philo->id);
			philo->dead = 1;
			pthread_mutex_unlock(&philo->mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->mutex);
		pthread_mutex_lock(&philo->left_fork);
		pthread_mutex_lock(philo->right_fork);
		printf("%lld %d has taken a fork\n", timestamp(philo->starting_time), philo->id);
		pthread_mutex_lock(&philo->mutex);
		printf("%lld %d is eating\n", timestamp(philo->starting_time), philo->id);
		usleep(philo->time_to_eat * 1000); // Simulate eating
		philo->last_meal = timestamp(philo->starting_time); // Update last meal time
		pthread_mutex_unlock(&philo->mutex);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(&philo->left_fork);
		printf("%lld %d is sleeping\n", timestamp(philo->starting_time), philo->id);
		usleep(philo->time_to_sleep * 1000); // Simulate sleeping
		printf("%lld %d is thinking\n", timestamp(philo->starting_time), philo->id);
	}
	return (NULL);
}

void assign_right_forks(t_data *data)
{
	int i = 0;

	while (i < data->nbr_of_philosophers)
	{
		if (i + 1 < data->nbr_of_philosophers)
			data->philosophers[i].right_fork = &data->philosophers[i + 1].left_fork;
		else
			data->philosophers[i].right_fork = &data->philosophers[0].left_fork;
		i++;
	}
}

int	init_philo_data(t_data *data, int argc, char **argv)
{
	time_t	starting_time;
	int		i;

	i = 0;
	data->nbr_of_philosophers = ft_atoi(argv[1]);
	starting_time = init_time();
	while (i < data->nbr_of_philosophers)
	{
		data->philosophers[i].max_meals = 0;
		if (argc == 6)
			data->philosophers[i].max_meals += ft_atoi(argv[5]);
		data->philosophers[i].dead = 0;
		data->philosophers[i].time_to_die = ft_atoi(argv[2]);
		data->philosophers[i].time_to_eat = ft_atoi(argv[3]);
		data->philosophers[i].time_to_sleep = ft_atoi(argv[4]);
		data->philosophers[i].id = i + 1;
		data->philosophers[i].data = data;
		data->philosophers[i].starting_time = starting_time;
		data->philosophers[i].last_meal = starting_time;
		i++;
	}
	return (1);
}

void *monitor(void *arg)
{
	t_data *data = (t_data *)arg;
	int i;
	long long current_time;

	while (data->simulation_running)
	{
		i = 0;
		while (i < data->nbr_of_philosophers)
		{
			pthread_mutex_lock(&data->philosophers[i].mutex);
			current_time = timestamp(data->philosophers[i].starting_time);
			if (current_time - data->philosophers[i].last_meal >= data->philosophers[i].time_to_die)
			{
				printf("%lld %d died\n", current_time, data->philosophers[i].id);
				data->philosophers[i].dead = 1;
				data->simulation_running = 0;
				pthread_mutex_unlock(&data->philosophers[i].mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philosophers[i].mutex);
			i++;
		}
	}
	return (NULL);
}


int	init_threads(t_data *data)
{
	int			i;
	pthread_t	monitor_tid;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL, run_philo, &data->philosophers[i]) != 0)
		{
			error_message("Thread creation failed", NULL);
			return (0);
		}
		i++;
	}
	if (pthread_create(&monitor_tid, NULL, monitor, data) != 0)
	{
		error_message("Monitor thread creation failed", NULL);
		return (0);
	}
	pthread_join(monitor_tid, NULL);
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
int init_data(t_data *data, int argc, char **argv)
{
	int i;

	data->error = 0;
	data->simulation_running = 1;
	data->nbr_of_philosophers = ft_atoi(argv[1]);
	data->philosophers = malloc(data->nbr_of_philosophers * sizeof(t_philosopher));
	if (!data->philosophers)
	{
		error_message("Malloc fail!", NULL);
		return (0);
	}
	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		if (pthread_mutex_init(&data->philosophers[i].left_fork, NULL) != 0 ||
			pthread_mutex_init(&data->philosophers[i].mutex, NULL) != 0)
		{
			error_message("Mutex init failed", NULL);
			return (0);
		}
		i++;
	}
	assign_right_forks(data);
	if (!init_philo_data(data, argc, argv))
		free_all(data);
	return (1);
}

static int	arg_check(int argc)
{
	if (argc > 6 || argc < 5)
	{
		ft_putendl_fd("ERROR!", 2);
		ft_putstr_fd("Input arguments like this: ", 2);
		ft_putstr_fd(ARG1, 2);
		ft_putstr_fd(ARG2, 2);
		ft_putstr_fd(ARG3, 2);
		ft_putstr_fd(ARG4, 2);
		ft_putendl_fd(ARG5, 2);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_data			data;

	if (!arg_check(argc) || !init_data(&data, argc, argv) || !validate_input(&data, argv))
		return (1);
	if (!init_threads(&data))
	{
		free_all(&data);
		return (0);
	}
	wait_for_finish(&data);
	destroy_left_forks(&data);
	free_all(&data);
	return (0);
}
