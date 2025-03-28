/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msuokas <msuokas@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 10:19:55 by msuokas           #+#    #+#             */
/*   Updated: 2025/03/28 15:29:00 by msuokas          ###   ########.fr       */
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

void	*run_philo(void* arg)
{
	t_thread_args	*args;
	t_philosopher	*philo;
	int				meals;

	args = (t_thread_args*)arg;
	philo = args->philo;
	meals = 0;
	while(!philo->dead)
	{
		if (philo->max_meals > 0)
		{
			if (meals == philo->max_meals)
				break ;
		}
		if (timestamp(philo->starting_time) - philo->last_meal >= philo->time_to_die)
		{
			print_status(args->mutex_msg, "dead", timestamp(philo->starting_time), philo->id);
			free(args);
			return (NULL);
		}
		else
		{
			print_status(args->mutex_msg, "thinking", timestamp(philo->starting_time), philo->id);
			pthread_mutex_lock(philo->left_fork);
			philo->left_fork_pickup_time = timestamp(philo->starting_time);
			if (timestamp(philo->starting_time) - philo->left_fork_pickup_time >= philo->time_to_die)
			{
				pthread_mutex_unlock(philo->left_fork);
				print_status(args->mutex_msg, "dead", timestamp(philo->starting_time), philo->id);
				free(args);
				return (NULL);
			}
			print_status(args->mutex_msg, "fork", timestamp(philo->starting_time), philo->id);
			pthread_mutex_lock(philo->right_fork);
			print_status(args->mutex_msg, "eating", timestamp(philo->starting_time), philo->id);
			meals++;
			usleep(philo->time_to_eat * 1000);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			philo->last_meal = timestamp(philo->starting_time);
			print_status(args->mutex_msg, "sleeping", timestamp(philo->starting_time), philo->id);
			usleep(philo->time_to_sleep * 1000);
		}
	}
	free(args);
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
		if (pthread_mutex_init(data->philosophers[i].left_fork, NULL) != 0)
		{
			error_message("Initialization failed!\n", "Mutex");
			return (0);
		}
		i++;
	}
	return (1);
}

void	destroy_left_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_of_philosophers)
	{
		if (pthread_mutex_destroy(data->philosophers[i].left_fork) != 0)
			error_message("Destruction failed", "Mutex");
		free(data->philosophers[i].left_fork);
		i++;
	}
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

int init_threads(t_data *data)
{
	int				i;
	time_t			starting_time;
	t_thread_args	*args;

	i = 0;
	starting_time = init_time();
	if (init_left_forks(data))
		assign_right_forks(data);
	while (i < data->nbr_of_philosophers)
	{
		args = malloc(sizeof(t_thread_args));
		args->philo = &data->philosophers[i];
		args->mutex_msg = data->status_msg;
		data->philosophers[i].starting_time = starting_time;
		if (pthread_create(&data->philosophers[i].thread, NULL, run_philo, args) != 0)
		{
			printf("Thread creation failed for philosopher %d\n", data->philosophers[i].id);
			free(args);
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

int	arg_check(t_data *data, int argc, char **argv)
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
	init_data(data);
	if (!validate_input(data, argv))
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data			data;

	if (!arg_check(&data, argc, argv))
		return (1);
	init_philo_data(&data, argc, argv);
	init_threads(&data);
	wait_for_finish(&data);
	free_all(&data);
	return (0);
}
