/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:19:55 by msuokas           #+#    #+#             */
/*   Updated: 2025/04/03 15:56:45 by msuokas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*run_philo(void *arg)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)arg;
	if (philo->data->nbr_of_philosophers == 1)
	{
		think_lock(philo);
		single_eat_lock(philo);
		return (NULL);
	}
	while (!philo->dead && philo->data->simulation_running)
	{
		think_lock(philo);
		eat_lock(philo);
		sleep_lock(philo);
		if (philo->max_meals > 0 && philo->meals >= philo->max_meals)
			break;
	}
	return (NULL);
}

void assign_right_forks(t_data *data)
{
	int	i;

	i = 0;
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
		data->philosophers[i].max_meals = -1;
		if (argc == 6)
			data->philosophers[i].max_meals = ft_atoi(argv[5]);
		data->philosophers[i].dead = 0;
		data->philosophers[i].time_to_die = ft_atoi(argv[2]);
		data->philosophers[i].time_to_eat = ft_atoi(argv[3]);
		data->philosophers[i].time_to_sleep = ft_atoi(argv[4]);
		data->philosophers[i].id = i + 1;
		data->philosophers[i].data = data;
		data->philosophers[i].starting_time = starting_time;
		data->philosophers[i].last_meal = starting_time;
		data->philosophers[i].meals = 0;
		i++;
	}
	return (1);
}

void *monitor(void *arg)
{
	t_data		*data;
	int			i;
	long long	current_time;
	long long	last_meal;
	int			time_to_die;

	data = (t_data *)arg;
	while (data->simulation_running)
	{
		i = 0;
		while (i < data->nbr_of_philosophers)
		{
			pthread_mutex_lock(&data->action_lock);
			current_time = timestamp(data->philosophers[i].starting_time);
			last_meal = data->philosophers[i].last_meal; // 🕒 Atomic read
			time_to_die = data->philosophers[i].time_to_die;
			pthread_mutex_unlock(&data->action_lock);
			if (current_time - last_meal >= time_to_die)
			{
				pthread_mutex_lock(&data->action_lock);
				if (data->simulation_running) // Prevent duplicate death prints
					printf("%lld %d died\n", current_time, data->philosophers[i].id);
				data->simulation_running = 0;
				pthread_mutex_unlock(&data->action_lock);
				return (NULL);
			}
			i++;
		}
		usleep(100);
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
	int	i;

	data->error = 0;
	data->simulation_running = 1;
	data->nbr_of_philosophers = ft_atoi(argv[1]);
	if (data->nbr_of_philosophers > 0)
		data->philosophers = malloc(data->nbr_of_philosophers * sizeof(t_philosopher));
	else
	{
		error_message(" <- Number of philosophers cannot be negative!", argv[1]);
		return (0);
	}
	pthread_mutex_init(&data->action_lock, NULL);
	data->meals_required = (argc == 6) ? ft_atoi(argv[5]) : -1;
	data->meals_completed = 0;
	pthread_mutex_init(&data->meal_lock, NULL);
	if (!data->philosophers)
	{
		error_message("Malloc fail!", NULL);
		return (0);
	}
	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		if (pthread_mutex_init(&data->philosophers[i].left_fork, NULL) != 0)
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
	t_data	data;

	if (!arg_check(argc) || !init_data(&data, argc, argv) || !validate_input(&data, argv))
		return (1);
	if (!init_threads(&data))
	{
		free_all(&data);
		return (0);
	}
	wait_for_finish(&data);
	destroy_mutexes(&data);
	free_all(&data);
	return (0);
}
